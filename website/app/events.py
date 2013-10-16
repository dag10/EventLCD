import requests
from datetime import datetime
import pytz
import urllib
import yaml

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
    events = filter(
        lambda e: 'location' in e and location.lower() in e['location'].lower(),
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
        'fields': 'items(location, start, summary)',
        'key': self.config['token'],
    }

    url = (
        'https://www.googleapis.com/calendar/v3/calendars/{0}/events?{1}'
        ).format(calendar, urllib.urlencode(params))

    response = requests.get(url);

    if response.status_code is not 200:
      raise Exception('Google HTTP %d' % response.status_code)

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
      e['start'] = e['start']['dateTime']

    return e

