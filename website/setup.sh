#!/bin/bash

# Install Flask
python2.7 virtualenv.py flask
flask/bin/pip install flask==0.10
flask/bin/pip install tornado
flask/bin/pip install --upgrade google-api-python-client
flask/bin/pip install pytz
flask/bin/pip install PyYAML
flask/bin/pip install requests
flask/bin/pip install python-dateutil

