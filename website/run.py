#!flask/bin/python
import yaml
from app import app
from tornado.wsgi import WSGIContainer
from tornado.ioloop import IOLoop
from tornado.web import FallbackHandler, Application

try:
  with open('app.yaml', 'r') as f:
    config = yaml.load(f)
except:
  print 'Missing/invalid config (app.yaml)'
  quit()

#app.run(debug=True, port=config['port'], host=config['host'])

application = Application([
  (r'.*', FallbackHandler, dict(fallback=WSGIContainer(app)))])

application.listen(config['port'])
IOLoop.instance().start()

