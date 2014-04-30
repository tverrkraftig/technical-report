from flask import Flask
from flask.ext import restful
import resources

app = Flask(__name__)
api = restful.Api(app)

api.add_resource(resources.Status, '/status/<string:id>')
api.add_resource(resources.StatusOptions, '/status')
api.add_resource(resources.Data, '/data/<string:id>/<string:sensor>')
api.add_resource(resources.DataOptions, '/data')
api.add_resource(resources.Data_Collection, '/data/<string:id>')
api.add_resource(resources.Command, '/command/<string:id>')
api.add_resource(resources.CommandOptions, '/command')

if __name__ == '__main__':
    app.run(debug=True)
