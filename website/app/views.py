from app import app
import json
from events import Events
from datetime import datetime
import pytz


@app.route('/location/<location>/events')
@app.route('/location/<location>/events/<int:num>')
def index(location, num=3):
  response = {}
  response['time'] = datetime.utcnow().replace(
      microsecond=0, tzinfo=pytz.utc).isoformat('T')

  try:
    events = Events()
    response['events'] = events.getNextEvents(location, num)
  except Exception as e:
    response = {'error': str(e)}

  return json.dumps(response)

