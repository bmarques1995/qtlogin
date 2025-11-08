from rest_framework import serializers
from authapp.models import User

class UserSerializer(serializers.ModelSerializer):
    class Meta:
        model= User
        fields= ['name', 'subscription', 'confirmed_registration']
    
    def __init__(self, *args, **kwargs):
        kwargs['many'] = kwargs.get('many', True)
        super().__init__(*args, **kwargs)