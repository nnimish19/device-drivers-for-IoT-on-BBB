import django_tables2 as tables
from dashboard.models import Person

class PersonTable(tables.Table):
    class Meta:
        model = Person
        # add class="paleblue" to <table> tag
        attrs = {"class": "paleblue"}