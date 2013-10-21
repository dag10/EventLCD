#!/bin/bash

# Install Flask
python virtualenv.py flask
flask/bin/pip install flask==0.9
flask/bin/pip install --upgrade google-api-python-client
flask/bin/pip install pytz
flask/bin/pip install PyYAML
flask/bin/pip install requests
flask/bin/pip install python-dateutil

