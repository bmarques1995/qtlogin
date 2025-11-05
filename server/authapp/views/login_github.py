from django.conf import settings
from django.views.decorators.http import require_http_methods
from django.http import HttpRequest, HttpResponse, JsonResponse
from django.views.decorators.csrf import csrf_exempt
from Shared.github_auth import get_github_oauth_token, get_github_user

@csrf_exempt
@require_http_methods(['GET'])
def login_github(request: HttpRequest):
    query_params = request.GET.dict()

    token_data = get_github_oauth_token(query_params)
    #return JsonResponse(token_data)
    user_data = get_github_user(token_data['access_token'][0])
    return JsonResponse(user_data, status=200)