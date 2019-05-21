from django.urls import path

from database.views import detail
from database.views import submit
from database.views import add_record


app_name = "database"
urlpatterns = [
    path("detail", detail, name="detail"),
    path("submit", submit, name="submit"),
    path("add_record", add_record, name="add_record"),
]
