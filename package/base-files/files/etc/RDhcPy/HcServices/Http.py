import requests
from requests.exceptions import HTTPError
from requests.structures import CaseInsensitiveDict


class Http:
    def create_new_http_header(self, domitory_id: str = "", cookie: str = ""):
        new_http_header = CaseInsensitiveDict()
        new_http_header["Accept"] = "application/json"
        new_http_header["X-DormitoryId"] = domitory_id
        new_http_header["Cookie"] = cookie
        new_http_header["User-Agent"] = "RANG DONG"
        return new_http_header

    def get(self, url, headers):
        resp = None
        try:
            resp = requests.request("GET", url, headers=headers)
            return resp
        except HTTPError as err:
            return ""
        except Exception as err:
            return ""

    def post(self, url, headers, data):
        try:
            resp = requests.request("POST", url, headers=headers, json=data)
            return resp
        except HTTPError as err:
            return ""
        except Exception as err:
            return ""

    def put(self, url, headers, data):
        resp = None
        try:
            resp = requests.request("PUT", url, headers=headers, json=data)
            return resp
        except HTTPError as err:
            return ""
        except Exception as err:
            return ""

    async def delete(self, url, headers):
        resp = None
        try:
            resp = requests.request("DELETE", url, headers=headers)
            return resp
        except HTTPError as err:
            return ""
        except Exception as err:
            return ""
