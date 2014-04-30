from flask import Response
from functools import wraps
from helpers import unicode_to_str

def get_str_object_or_404(action):
    @wraps(action)
    def wrapper(*args, **kwargs):
        result = action(*args, **kwargs)
        if not result:
            return {}, 404, {'Access-Control-Allow-Origin': '*', 'Access-Control-Allow-Headers': 'accept, content-type, origin'}
        else:
            return unicode_to_str(result), 200, {'Access-Control-Allow-Origin': '*', 'Access-Control-Allow-Headers': 'accept, content-type, origin'}
    return wrapper
