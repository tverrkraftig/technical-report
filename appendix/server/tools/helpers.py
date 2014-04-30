import time

def unicode_to_str(data):
    if isinstance(data, dict):
        ret = {}
        for key, value in data.iteritems():
            ret[unicode_to_str(key)] = unicode_to_str(value)
        return ret
    elif isinstance(data, list):
        ret = []
        for value in data:
            ret.append(unicode_to_str(value))
        return ret
    else:
        return str(data)

def get_microtime():
    return int(round(time.time() * 1000))
