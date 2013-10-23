from app import app
import json
from flask import request
from events import Events
from datetime import datetime
from datetime import date
import pytz
from pytz import timezone

today_only = True

@app.route('/location/<location>/events')
def index(location):
  # Parse HTTP request headers
  location_name = request.headers.get('X-Location-Name') or location
  rows = request.headers.get('X-LCD-Rows') or 4
  cols = request.headers.get('X-LCD-Cols') or 20
  rows = int(rows)
  cols = int(cols)

  # TEMP
  if location == 'all':
    events = Events().getNextEvents('lounge', 20)
    return json.dumps(events)

  # How many events based on whether there's enough rows to display the
  # information row.
  num_events = rows - 1 if rows > 1 else 1

  events = []
  error_str = None
  
  # Fetch today's events
  try:
    events = Events().getNextEvents(location, num_events)
    if today_only:
      events = filter(
          lambda e: e['start'].date() == date.today(),
          events)
  except Exception as e:
    error_str = 'Error: ' + str(e).replace('\n', ' ')

  # Current time string
  time = datetime.now().strftime('%H:%M')

  output = ''

  # If we have room for it, display the location name and current time.
  if rows > 1 and cols >= len(time) + len(location_name) + 1:
    output += location_name.upper()
    output += (' ' * (cols - len(location_name) - len(time)))
    output += time

  # If there's an error, show the error. Otherwise, list the events.
  if error_str:
    for r in range(rows - 1):
      output += '\n' + error_str[r * cols : (r + 1) * cols]
  else:
    for e in range(len(events)):
      if eventIsNow(events[e]):
        line = 'NOW:'
      else:
        line = events[e]['start'].strftime('%H:%M')
      line += ' '
      line += events[e]['summary']
      output += '\n' + line[:cols]

  # If there are still rows but no more event, say so.
  if len(events) < rows - 1 and rows > 1 and not error_str:
    if cols >= 20:
      output += '\nNo more events today' 
    elif cols > 15:
      output += '\nNo more events'

  return output

def eventIsNow(event):
  """Checks if an event object is happening right now.

  Args:
    event: Object with 'start' and 'end' datetimes or dates.

  Returns:
    Whether the event is now.
  """
  if 'start' not in event or 'end' not in event:
    return False

  # Since Google returns all datetimes with the timezone of the calendar,
  # and we're assuming this server is running in the same timezone,
  # equalize all the timezones for time comparison.
  start = event['start'].replace(tzinfo=pytz.utc)
  end = event['end'].replace(tzinfo=pytz.utc)

  return start <= datetime.now().replace(tzinfo=pytz.utc) <= end

