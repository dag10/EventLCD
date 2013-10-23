import requests
from datetime import datetime
import dateutil.parser
import pytz
import urllib
import yaml
import dateutil.parser
import re

class Events:
  def __init__(self):
    try:
      with open('app.yaml', 'r') as f:
        self.config = yaml.load(f)
    except:
      raise Exception('Missing/invalid config')

  def getNextEvents(self, location, num):
    """Gets the next events at a location.

    The location is a partial string match.

    Args:
      location: String of location.
      num: Number of events to retrieve.

    Returns:
      List of events.
    """
    events = self.getEvents(20)

    # Filter by location
    events = filter(
        lambda e: 'location' in e and location.lower() in e['location'].lower(),
        events)

    # Filter out all-day events
    events = filter(
        lambda e: 'start' in e and 'time' in dir(e['start']),
        events)

    return events[:num]


  def getEvents(self, num):
    """Gets the a list of events from the calendar.

    Args:
      num: Number of events to retrieve.

    Returns:
      List of events, or an empty list.

    Throws:
      Exception if an error occurs.
    """
    calendar = self.config['calendar_id']
    isotime = datetime.utcnow().replace(
        microsecond=0, tzinfo=pytz.utc).isoformat('T')

    params = {
        'maxResults': num,
        'orderBy': 'startTime',
        'showDeleted': False,
        'singleEvents': True,
        'timeMin': isotime,
        'fields': 'items(location, start, end, summary, description)',
        'key': self.config['token'],
    }

    url = (
        'https://www.googleapis.com/calendar/v3/calendars/{0}/events?{1}'
        ).format(calendar, urllib.urlencode(params))

    response = requests.get(url);

    if response.status_code is not 200:
      raise Exception('Google %d' % response.status_code)

    events = response.json()['items']
    events = map(self.formatEvent, events)
    return events


  def formatEvent(self, e):
    """Formats an event dictionary from Google's format to our own.

    Args:
      e: Dictionary with Google's event structure.

    Returns:
      Restructured dictionary.
    """
    if 'start' in e and 'dateTime' in e['start']:
      e['start'] = dateutil.parser.parse(e['start']['dateTime'])

    if 'end' in e and 'dateTime' in e['end']:
      e['end'] = dateutil.parser.parse(e['end']['dateTime'])

    # If event description contains special tag [lcd: <value>], display that
    # value instead of the event summary.
    if 'description' in e:
      lcdValue = re.findall(r'\[lcd:\s*([^\]]+)\]', e['description'])
      if lcdValue:
        e['summary'] = lcdValue[0]

    return e

