import requests
import json

url = "http://127.0.0.1:8000/update?uuid=123&distance=123"

# data = {'uuid': 'C001', 'dist':123}
# response =  requests.post(url, data=json.dumps(data))

response =  requests.get(url)

print("status code : ", response.content)