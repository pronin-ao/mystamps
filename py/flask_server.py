from flask import Flask
app = Flask(__name__)

import load_db as db

class Database:
    def __init__(self):
        self.data = db.load()

DB = Database()


@app.route("/")
def hello():
    return "<h1 style='color:blue'>STAMPS!!!</h1>"

@app.route("/get/<country>")
def get(country):
    return DB.data[country]


if __name__ == "__main__":
    app.run(host='0.0.0.0')
