import paho.mqtt.client as mqtt
import json
import sqlite3
import datetime
# SQLite DB Name
DB_Name =  "IoT.db"


class DatabaseManager():
    def __init__(self):
        self.conn = sqlite3.connect(DB_Name)
        self.conn.execute('pragma foreign_keys = on')
        self.conn.commit()
        self.cur = self.conn.cursor()

    def add_del_update_db_record(self, sql_query, args=()):
        self.cur.execute(sql_query, args)
        self.conn.commit()
        return

    def __del__(self):
        self.cur.close()
        self.conn.close()

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected successfully")
    else:
        print("Connect returned result code: " + str(rc))

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print("Received message: " + msg.topic + " -> " + msg.payload.decode("utf-8"))
    dbObj = DatabaseManager()
    if msg.topic == "esp/dht/temperature":
        dbObj.add_del_update_db_record(
            "insert into DHT22_Temperature_Data  (Date_n_Time, Temperature) values (?,?)",
            [  datetime.datetime.now().strftime("%d/%m/%Y %H:%M:%S"), msg.payload.decode("utf-8")])
    if msg.topic == "esp/dht/humidity":
        dbObj.add_del_update_db_record(
            "insert into DHT22_Humidity_Data  (Date_n_Time, Humidity) values (?,?)",
            [  datetime.datetime.now().strftime("%d/%m/%Y %H:%M:%S"), msg.payload.decode("utf-8")])
    del dbObj

# create the client

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

# enable TLS
client.tls_set(tls_version=mqtt.ssl.PROTOCOL_TLS)

# set username and password
client.username_pw_set("testest", "TestTest1")

# connect to HiveMQ Cloud on port 8883
client.connect("4be20a833dfa43c591120e0543b9f9e6.s1.eu.hivemq.cloud", 8883)

# subscribe to the topic "my/test/topic"
print(client.subscribe("esp/dht/temperature"))
print(client.subscribe("esp/dht/humidity"))

# publish "Hello" to the topic "my/test/topic"


# Blocking call that processes network traffic, dispatches callbacks and handles reconnecting.
client.loop_forever()