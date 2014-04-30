from flask import request
from flask.ext import restful
from pymongo import MongoClient
from tools.decorators import get_str_object_or_404
from tools.helpers import get_microtime, unicode_to_str

mongodb = MongoClient().db

class OptionsResrouce(restful.Resource):
    def options(self):
        return {'Allow': 'GET,POST'}, 200, {'Access-Control-Allow-Origin': '*', 'Access-Control-Allow-Methods': 'POST,GET', 'Access-Control-Allow-Headers': 'accept, content-type, origin'}

class Status(restful.Resource):
    def __init__(self):
        self.collection = mongodb.status

    @get_str_object_or_404
    def get(self, id):
        return self.collection.find_one({'device_id': id})

    def post(self, id):
        data = request.get_json(force=True, cache=False)
        data["device_id"] = id
        data["timestamp"] = get_microtime()

        self.collection.update({'device_id': id}, data, upsert=True)

        #return {"commands": Command().get(id)}
        return {"commands": []}

    def options(self, id):
        return {'Allow': 'GET,POST'}, 200, {'Access-Control-Allow-Origin': '*', 'Access-Control-Allow-Methods': 'POST,GET', 'Access-Control-Allow-Headers': 'accept, content-type, origin'}

class StatusOptions(OptionsResrouce):
    pass

class Data(restful.Resource):
    def __init__(self):
        self.collection = mongodb.data

    @get_str_object_or_404
    def get(self, id, sensor):
        return self.collection.find_one({'device_id': id, 'sensor': sensor})

    def post(self, id, sensor):
        data = request.get_json(force=True, cache=False)

        data["device_id"] = id
        data["timestamp"] = get_microtime()
        data["sensor"] = sensor
        self.collection.update({'device_id': id, 'sensor': sensor}, data, upsert=True)

        #return {"commands": Command().get(id)}
        return {"commands": []}

    def options(self, id):
        return {'Allow': 'GET,POST'}, 200, {'Access-Control-Allow-Origin': '*', 'Access-Control-Allow-Methods': 'POST,GET', 'Access-Control-Allow-Headers': 'accept, content-type, origin'}

class DataOptions(OptionsResrouce):
    pass

class Data_Collection(restful.Resource):
    def __init__(self):
        self.collection = mongodb.data

    @get_str_object_or_404
    def get(self, id):
        return [sensor for sensor in self.collection.find({'device_id': id})]

    def post(self, id):
        data = request.get_json(force=True, cache=False)

        for sensor_data in data:
            sensor_data["device_id"] = id
            sensor_data["timestamp"] = get_microtime()
            self.collection.update({'device_id': id, 'sensor': sensor_data['sensor']}, sensor_data, upsert=True)

        #return {"commands": Command().get(id)}
        return {"commands": []}

    def options(self, id):
        return {'Allow': 'GET,POST'}, 200, {'Access-Control-Allow-Origin': '*', 'Access-Control-Allow-Methods': 'POST,GET', 'Access-Control-Allow-Headers': 'accept, content-type, origin'}

class Command(restful.Resource):
    def __init__(self):
        self.collection = mongodb.commands
        self.id = hex(id(self))

    def __get_document_lock(self, id):
        document = self.collection.find_one({"device_id": id})

        if not document:
            self.collection.insert({"device_id": id, "state": self.id, "queue": []})
            document = self.collection.find_one({"device_id": id})

        while document["state"] != self.id:
            while document["state"] != "ready":
                document = self.collection.find_one({"device_id": id})
            self.collection.update({"device_id": id, "state": "ready"}, {"$set": {"state": self.id}})
            document = self.collection.find_one({"device_id": id})

    def __free_document_lock(self, id):
        self.collection.update({"device_id": id}, {"$set": {"state": "ready"}})

    def get(self, id):
        self.__get_document_lock(id)

        try:
            document = self.collection.find_one({"device_id": id})
            self.collection.update({"device_id": id}, {"$set": {"queue": []}})
        finally:
            self.__free_document_lock(id)

        return unicode_to_str(document["queue"])

    def post(self, id):
        command = request.get_json(force=True, cache=False)
        command["timestamp"] = get_microtime()

        self.__get_document_lock(id)

        try:
            self.collection.update({"device_id": id}, {"$push": {"queue": command}})
        finally:
            self.__free_document_lock(id)
        return {}, 200, {'Access-Control-Allow-Origin': '*', 'Access-Control-Allow-Headers': 'accept, content-type, origin'}

    def options(self, id):
        return {'Allow': 'GET,POST'}, 200, {'Access-Control-Allow-Origin': '*', 'Access-Control-Allow-Methods': 'POST,GET', 'Access-Control-Allow-Headers': 'accept, content-type, origin'}

class CommandOptions(OptionsResrouce):
    pass
