from django.urls import path

from foreground.views import welcome
from foreground.views import visit_admin
from foreground.views import visit_detail
from foreground.views import visit_submit


urlpatterns = [
    path("", welcome, name="welcome"),
    path("visit_admin", visit_admin, name="visit_admin"),
    path("visit_detail", visit_detail, name="visit_detail"),
    path("visit_submit", visit_submit, name="visit_submit"),
]
