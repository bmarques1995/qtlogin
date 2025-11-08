from functools import wraps
import json
from django.http import HttpRequest, JsonResponse

def validate_request_json_body():
    def decorator(view_func):
        @wraps(view_func)
        def _wrapped_view(request: HttpRequest, *args, **kwargs):
            try:
                request.json_body = json.loads(request.body)
            except json.JSONDecodeError:
                return JsonResponse({'error': 'The json provided must be valid'}, status=401)
            return view_func(request, *args, **kwargs)
        return _wrapped_view
    return decorator