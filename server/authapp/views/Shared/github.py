from Shared.crypt import gen_password
from Shared.github_auth import get_github_oauth_token, retrieve_github_user_info
from authapp.serializers.github_id import GithubIDSerializer

def get_github_user(query_params):
    token_data = get_github_oauth_token(query_params)
    #return JsonResponse(token_data)
    return retrieve_github_user_info(token_data['access_token'][0])

def register_github_info(user_info, id):
    hashed_id = gen_password(id, rounds=11)
    cnpj_obj = {
        'user_idx': user_info.name,
        'github_id_hash': hashed_id
    }
    github_id_serializer = GithubIDSerializer(data = cnpj_obj)
    if github_id_serializer.is_valid():
        github_id_serializer.save()
