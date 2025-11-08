from rest_framework import serializers
from authapp.models import GithubID

class GithubIDSerializer(serializers.ModelSerializer):
    class Meta:
        model= GithubID
        fields= ['user_idx', 'github_id_hash']
    
    def __init__(self, *args, **kwargs):
        kwargs['many'] = kwargs.get('many', True)
        super().__init__(*args, **kwargs)