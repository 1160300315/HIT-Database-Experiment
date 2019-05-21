from django.db import models


class Teacher(models.Model):

    name = models.CharField(
        primary_key=True, max_length=200
    )
    email = models.EmailField(
        unique=True, max_length=200
    )
    position = models.CharField(max_length=200)


class Project(models.Model):

    name = models.CharField(
        primary_key=True, max_length=200
    )
    salary = models.PositiveIntegerField()
    coverage = models.PositiveIntegerField()


class Repository(models.Model):

    address = models.CharField(
        primary_key=True, max_length=200
    )
    capacity = models.PositiveSmallIntegerField()


class Facility(models.Model):

    id = models.PositiveSmallIntegerField(
        primary_key=True
    )
    type = models.PositiveSmallIntegerField(
        choices=(
            (0, '电脑'),
            (1, '硬件设施'),
            (2, '数据')
        )
    )
    repository = models.ForeignKey(
        to=Repository, to_field="address",
        on_delete=models.CASCADE, null=False
    )


class Graduate(models.Model):
    """
    指向自身 to 参数只能是字符串, 例如 self.
    """

    name = models.CharField(
        primary_key=True, max_length=200
    )
    level = models.PositiveSmallIntegerField(
        choices=(
            (0, "硕士一年级"),
            (1, "硕士二年级"),
            (2, "硕士三年级"),
            (3, "博士生")
        )
    )
    leader = models.ForeignKey(
        to="self", to_field="name",
        on_delete=models.SET_NULL, null=True,
    )


class BroughtIn(models.Model):
    """
    若想通过字段指向外键, 则必须设置主键.
    """

    id = models.PositiveIntegerField(primary_key=True)
    teacher = models.ForeignKey(
        to=Teacher, to_field="name",
        on_delete=models.SET_NULL, null=True
    )
    project = models.ForeignKey(
        to=Project, to_field="name",
        on_delete=models.CASCADE, null=False
    )
    facility = models.ForeignKey(
        to=Facility, to_field="id",
        on_delete=models.SET_NULL, null=True
    )
    date = models.DateTimeField()


class WorkOn(models.Model):

    id = models.PositiveIntegerField(primary_key=True)
    graduate = models.ForeignKey(
        to=Graduate, to_field="name",
        on_delete=models.CASCADE, null=False
    )
    repository = models.ForeignKey(
        to=Repository, to_field="address",
        on_delete=models.CASCADE, null=False
    )


class Record(models.Model):
    headline = models.CharField(
        null=True, blank=True, max_length=200
    )
    content = models.TextField(max_length=1000)
    date = models.DateTimeField()

    def __str__(self):
        """
        程序在后台管理界面会显示 headline.
        """

        return self.headline
