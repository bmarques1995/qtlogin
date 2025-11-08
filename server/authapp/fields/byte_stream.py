from django.db import models
from django.forms import ValidationError

class ByteStreamField(models.Field):
    def __init__(self, stream_size=0, *args, **kwargs):
        self.stream_size = stream_size
        super().__init__(*args, **kwargs)
    
    # Define the mapping for database types
    DB_TYPE_MAP = {
        'mysql': 'BINARY',
        'postgresql': 'BIGINT',  # PostgreSQL doesn't support unsigned, so default to BIGINT
        'sqlite': 'BIGINT',
        'oracle': 'NUMBER(19)',
    }

    def db_type(self, connection):
        return f'{self.DB_TYPE_MAP.get(connection.vendor, super().db_type(connection))}({self.stream_size})'
    
    def deconstruct(self):
        name, path, args, kwargs = super().deconstruct()
        kwargs['stream_size'] = self.stream_size
        return name, path, args, kwargs
    
    def to_python(self, value):
        if value is None:
            return value
        try:
            value = bytes(value)
        except (TypeError, ValueError):
            raise ValidationError('This field requires a byte stream.')
        return value
    
    def validate(self, value, model_instance):
        super().validate(value, model_instance)
        if self.stream_size < 1:
            raise ValidationError('Ensure this value is greater than or equal to 1.')
