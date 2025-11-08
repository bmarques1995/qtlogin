from rest_framework import serializers
from authapp.models import UserRefreshToken

class UserRefreshTokenSerializer(serializers.ModelSerializer):
    class Meta:
        model= UserRefreshToken
        fields= ['user_idx', 'refresh_token_hash', 'expiration']
    
    def __init__(self, *args, **kwargs):
        kwargs['many'] = kwargs.get('many', True)
        super().__init__(*args, **kwargs)
