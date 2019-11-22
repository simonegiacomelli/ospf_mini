import asyncio
import os
import platform
import threading
import time
import uuid
from threading import Thread
from typing import Dict, Set, Callable

from gmqtt import Client as MQTTClient

#.serverHost("mqtt.flespi.io")
#.simpleAuth(Mqtt5SimpleAuth.builder().username("3s897csODyMGcSwQ75LY7uTipFBIBnzsDvrBvHfZ6Pt6xQMsLnhGH0PVvetUrQcU").build())

class BrokerConfig:
    hostname = 'mqtt.flespi.io' 
    username = '3s897csODyMGcSwQ75LY7uTipFBIBnzsDvrBvHfZ6Pt6xQMsLnhGH0PVvetUrQcU'
    password = None
    port = 1883
    ssl = False
    from gmqtt.mqtt.constants import MQTTv50
    mqtt_version = MQTTv50

Subscriber = Callable[[str,str], None]

class GmqttClient:
    counter = 0

    def __init__(self, config: BrokerConfig = None):
        if config == None:
            config = BrokerConfig()
        self.config: BrokerConfig = config
        self.client = None
        self.thread = None
        self.all_listeners: Dict[str, Subscriber] = {}
        self.active_listeners: Set = set()
        self.loop = None
        self.ready = threading.Event()

    @property
    def is_connected(self):
        return self.client.is_connected

    def publish(self, topic: str, payload):
        # At most once (0)
        # At least once (1)
        # Exactly once (2).
        self.ready.wait()

        assert self.client, 'you need to activate connection'

        self.client.publish(topic, payload, qos=0, message_expiry_interval=10)

    def subscribe(self, topic: str, listener: Subscriber):
        if self.all_listeners.get(topic, None):
            raise Exception(f'Topic {topic} already registered')
        self.all_listeners[topic] = listener
        self.ready.wait()
        self.PROCESS.set()

    def unsubscribe(self, topic):
        listener = self.all_listeners.get(topic, None)
        if not listener:
            raise Exception(f'Topic {topic} was not registered')
        self.all_listeners.pop(topic)
        self.active_listeners.discard(topic)

    def connect(self):
        if self.client:
            return self

        self.thread = Thread(target=self._loop, daemon=True)
        self.thread.start()
        # print('ready received')
        return self

    def _loop(self):
        try:

            self.loop = asyncio.new_event_loop()
            self.loop.run_until_complete(self._connect())
            # asyncio.run(self._connect())

        except KeyboardInterrupt:
            if self.client:
                self.client.disconnect()
            # print("Received exit, exiting")

    async def _connect(self):
        GmqttClient.counter += 1
        client_id = f'client-id/{platform.node()}/pid_{os.getpid()}/{uuid.getnode()}/{GmqttClient.counter}'
        self.client = MQTTClient(client_id)
        self.PROCESS = asyncio.Event()

        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message
        self.client.on_disconnect = self.on_disconnect
        self.client.on_subscribe = self.on_subscribe
        # print('set')

        self.client.set_auth_credentials(self.config.username, self.config.password)

        while True:
            try:
                await self.client.connect(self.config.hostname
                                          , port=self.config.port
                                          , ssl=self.config.ssl
                                          , version=self.config.mqtt_version)
                break
            except Exception as ex:
                print('connect failed', ex)
                time.sleep(1)
        self.ready.set()

        while True:
            await self.PROCESS.wait()
            self.PROCESS.clear()
            # print('process signaled')
            self._sync_subscriptions()
            # await self.STOP.wait()
        await self.client.disconnect()

    def on_connect(self, client, flags, rc, properties):
        self.connected = True
        print('Connected', id(self))
        self.PROCESS.set()
        self._sync_subscriptions()

    def _sync_subscriptions(self):
        if not self.client.is_connected:
            return
        unregistered = self.all_listeners.keys() - self.active_listeners
        for topic in unregistered:
            self.client.subscribe(topic, qos=1)
            self.active_listeners.add(topic)

    def _no_subscriber(self, message):
        print('RECV MSG with no subscriber:', message.topic, message.payload)

    def on_message(self, client, topic, payload, qos, properties):

        s = self.all_listeners.get(topic, self._no_subscriber)
        s(topic, payload)

    def on_disconnect(self, client, packet, exc=None):
        self.connected = False
        self.active_listeners.clear()
        print('Disconnected', id(self))
        self.PROCESS.set()

    def on_subscribe(self, client, mid, qos):
        # print('SUBSCRIBED')
        pass

f = open('save-beacon-session.csv','w')

def handle_rssi_message(topic,payload):
    msg = payload.decode("utf-8")
    f.write(f'{msg}\n')
    f.flush()
    print(msg)
    #timestamp = time.time().split()
    #print(msg.split(',') + timestamp)


def main():
    client = GmqttClient()
    client.connect()
    client.subscribe('cyber/rssi', handle_rssi_message)
    client.thread.join()


if __name__ == "__main__":
    main()