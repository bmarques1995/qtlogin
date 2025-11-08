from rest_framework import serializers
from authapp.models import GoogleID

class GoogleIDSerializer(serializers.ModelSerializer):
    class Meta:
        model= GoogleID
        fields= ['user_idx', 'google_id_hash']
    
    def __init__(self, *args, **kwargs):
        kwargs['many'] = kwargs.get('many', True)
        super().__init__(*args, **kwargs)