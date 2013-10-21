from app import app
import json
from events import Events
from datetime import datetime
from datetime import date
import pytz
import dateutil.parser


@app.route('/location/<location>/events')
@app.route('/location/<location>/events/<int:num>')
def index(location, num=3):
  today_only = True

  rows = 4
  cols = 20

  events = []
  error_str = None
  
  try:
    events = Events().getNextEvents(location, num)
    if today_only:
      events = filter(
          lambda e: dateutil.parser.parse(e['start']).date() == date.today(),
          events)
  except Exception as e:
    error_str = 'Error: ' + str(e)

  time = datetime.now().strftime('%H:%M')

  output = ''

  if rows > 1 and cols >= len(time) + len(location) + 1:
    output += location.upper()
    output += (' ' * (cols - len(location) - len(time)))
    output += time

  if error_str:
    for r in range(rows - 1):
      output += '\n' + error_str[r * cols : (r + 1) * cols]
  else:
    for e in range(min(len(events), rows - 1)):
      line = dateutil.parser.parse(events[e]['start']).strftime('%H:%M')
      line += ' '
      line += events[e]['summary']
      output += '\n' + line[:cols]

  if len(events) < rows - 1 and rows > 1:
    output += '\nNo more events today'

  return output

