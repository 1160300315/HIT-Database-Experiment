from django.contrib import admin

from database.models import Teacher
from database.models import Project
from database.models import Repository
from database.models import Facility
from database.models import Graduate
from database.models import BroughtIn
from database.models import WorkOn
from database.models import Record


admin.site.register(Teacher)
admin.site.register(Project)
admin.site.register(Repository)
admin.site.register(Facility)
admin.site.register(Graduate)
admin.site.register(BroughtIn)
admin.site.register(WorkOn)
admin.site.register(Record)
