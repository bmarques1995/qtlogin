from authapp.models.user import User
from authapp.serializers.user import UserSerializer


def register_user(name, subscription):
    treated_data = {
        'name': name,
        'subscription': subscription,
        'confirmed_registration': True
    }
    user: User
    user_serializer = UserSerializer(data= treated_data)
    if user_serializer.is_valid(raise_exception=True):
        user = user_serializer.save()

    return user