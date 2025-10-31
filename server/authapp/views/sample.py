from django.views.decorators.http import require_http_methods
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt

@csrf_exempt
@require_http_methods(['POST'])
def sample_route(request):
    return JsonResponse({'msg': 'connection established'}, status=200)