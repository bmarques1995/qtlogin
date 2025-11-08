from django.db import models
from .user import User
from authapp.fields.byte_stream import ByteStreamField

class UserRefreshToken(models.Model):
    user_idx = models.ForeignKey(to=User, on_delete=models.CASCADE)
    refresh_token_hash = ByteStreamField(stream_size=64, unique=True)
    expiration  = models.DateTimeField(auto_now= True)
    