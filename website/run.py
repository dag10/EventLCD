#!flask/bin/python
import yaml
from app import app

try:
  with open('app.yaml', 'r') as f:
    config = yaml.load(f)
except:
  print 'Missing/invalid config (app.yaml)'
  quit()

app.run(debug=True, port=config['port'], host=config['host'])

