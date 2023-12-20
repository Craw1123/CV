#!/usr/bin/env python
# coding: utf-8

# In[1]:


import requests
from Singleton import Logger
class ForecastException(Exception):
    def __init__(self, code=0, message='Unexpected error'):

        self.code = code
        self.message = message
    def __str__(self):
        return "Forecast API Exception: [{}] {}".format(self.code, self.message)
class Forecast:

    API_KEY = '2fe14a2a42905eb8800e935c97796e26'
    API_URL = "http://api.openweathermap.org/data/2.5/weather?q={}&units=metric&appid={}"
    def today(self, city):
        try:
            Logger().normal("City " + city + " was requested")
            response = requests.get(Forecast.API_URL.format(city, Forecast.API_KEY)).json()
            if response['cod'] != 200:
                raise ForecastException(response['cod'], response['message'])
            Logger().normal("Response passed!")
            return response
        except (requests.RequestException, ForecastException) as err:
            Logger().error(str(err))
            return None
class AuthForecast:

    class Decorators(object):

        @classmethod
        def auth_need(cls, func):

            def wrapper(*args, **kwargs):
                if args[0].auth():
                    Logger().normal("User {} called {}".format(args[0].name, func.__name__))
                    return func(*args, **kwargs)
                else:
                    Logger().warning("User is not authentificated")
                    return -1
            return wrapper
    def __init__(self):
 
        self._name = ''
        self._Forecast = Forecast()
    @property
    def name(self):

        return self._name
    def auth(self):

        if self._name:
            return 1
        while True:
            name = input("Auth Widget v.100500\nYou shouldn't be 'lol'\n'~' to cancel\nEnter the username: ")
            if name == 'lol':
                Logger().error("User {} INVALID".format(name))
                return 0
            elif name == '~':
                Logger().warning("User canceled auth".format(name))
                return 0
            else:
                self._name = name
                Logger().normal("User {} is authentificated".format(name))
                return 1
    @Decorators.auth_need
    def today(self, city):
        return self._Forecast.today(city)
def pretty_print(forecast):

    if forecast is None:
        Logger().warning("Forecast is None")
        print("No forecast for you")
        return
    if forecast == -1:
        Logger().error("User is not authentificated")
        print("You is not valid user")
        return
    print('FORECAST FOR TODAY:\n'
            + 'City: ' + str(forecast['name']) + '\n'
            + 'Hymidity: ' + str(forecast['main']['humidity']) + '%\n'
            + 'Temperature: ' + str(forecast['main']['temp']) + 'C\n'
            + 'Pressure: ' + str(forecast['main']['pressure']) + 'mBar\n')
if __name__ == '__main__':
    while True:
        city = input("\nEnter your city and get today forecast (ONLY FOR MEMBERS!!!)\n'~' - exit\nCity: ")
    if city == '~':
        break
    pretty_print(AuthForecast().today(city))
    Logger().print()


# In[ ]:




