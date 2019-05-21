from django.shortcuts import render
from django.http import HttpResponseRedirect
from django.urls import reverse
from django.utils import timezone

from database.models import Teacher
from database.models import Project
from database.models import Repository
from database.models import Facility
from database.models import Graduate
from database.models import BroughtIn
from database.models import WorkOn
from database.models import Record


def detail(request):
    """
    显示实验室所有的信息, 例如老师, 项目等.
    """

    teacher_list = Teacher.objects.order_by("-name")
    project_list = Project.objects.order_by("-coverage")
    repository_list = Repository.objects.order_by("-capacity")
    facility_list = Facility.objects.order_by("-id")
    graduate_list = Graduate.objects.order_by("-level")
    brought_in_list = BroughtIn.objects.order_by("-date")
    work_on_list = WorkOn.objects.order_by("-id")

    context = {
        'teacher_list': teacher_list,
        'project_list': project_list,
        'repository_list': repository_list,
        'facility_list': facility_list,
        'graduate_list': graduate_list,
        'brought_in_list': brought_in_list,
        'work_on_list': work_on_list
    }
    return render(request, 'detail.html', context)


def submit(request):
    return render(request, "submit.html")


def add_record(request):
    headline = request.POST["headline"]
    content = request.POST["content"]

    if len(headline) > 0:
        Record(
            headline=headline,
            content=content,
            date=timezone.now()
        ).save()
    return HttpResponseRedirect(reverse("database:submit"))

