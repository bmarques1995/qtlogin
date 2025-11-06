from django.conf import settings
from django.views.decorators.http import require_http_methods
from django.http import HttpRequest, HttpResponse, JsonResponse
from django.views.decorators.csrf import csrf_exempt
from .Shared.github import get_github_user

@csrf_exempt
@require_http_methods(['GET'])
def login_github(request: HttpRequest):
    query_params = request.GET.dict()
    #return JsonResponse(token_data)
    user_data = get_github_user(query_params)
    return JsonResponse(user_data, status=200)