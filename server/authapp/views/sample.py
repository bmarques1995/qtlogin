from django.views.decorators.http import require_http_methods
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt

from authapp.decorators import validate_refresh_token

@csrf_exempt
@require_http_methods(['POST'])
def sample_public_route(request):
    return JsonResponse({'msg': 'connection established'}, status=200)

@csrf_exempt
@require_http_methods(['POST'])
@validate_refresh_token()
def sample_private_route(request):
    return JsonResponse({'msg': 'connection established'}, status=200)