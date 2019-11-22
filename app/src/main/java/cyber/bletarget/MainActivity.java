package cyber.bletarget;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothProfile;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;

import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.google.android.material.snackbar.Snackbar;

import android.util.Log;
import android.view.View;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.ui.AppBarConfiguration;
import androidx.navigation.ui.NavigationUI;

import com.google.android.material.navigation.NavigationView;
import com.hivemq.client.mqtt.datatypes.MqttQos;
import com.hivemq.client.mqtt.mqtt5.Mqtt5BlockingClient;
import com.hivemq.client.mqtt.mqtt5.Mqtt5Client;
import com.hivemq.client.mqtt.mqtt5.message.auth.Mqtt5SimpleAuth;

import androidx.drawerlayout.widget.DrawerLayout;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import android.view.Menu;
import android.widget.Toast;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    private AppBarConfiguration mAppBarConfiguration;
    private BluetoothAdapter mBTAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        setupBluetooth();

        FloatingActionButton fab = findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                beaconManager.connectBeacons();
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
            }
        });
        DrawerLayout drawer = findViewById(R.id.drawer_layout);
        NavigationView navigationView = findViewById(R.id.nav_view);
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.
        mAppBarConfiguration = new AppBarConfiguration.Builder(
                R.id.nav_home, R.id.nav_gallery, R.id.nav_slideshow,
                R.id.nav_tools, R.id.nav_share, R.id.nav_send)
                .setDrawerLayout(drawer)
                .build();
        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment);
        NavigationUI.setupActionBarWithNavController(this, navController, mAppBarConfiguration);
        NavigationUI.setupWithNavController(navigationView, navController);
    }

    private void setupBluetooth() {
        mBTAdapter = BluetoothAdapter.getDefaultAdapter(); // get a handle on the bluetooth radio

        // Ask for location permission if not already allowed
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED)
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.ACCESS_COARSE_LOCATION}, 1);

    }

    private void testMqtt() {
        try {
            testMqttInternal();
        } catch (Exception ex) {
            Toast.makeText(getApplicationContext(), "exception" + ex.getMessage(), Toast.LENGTH_LONG).show();
        }
    }

    private void testMqttInternal() throws InterruptedException {
        final Mqtt5BlockingClient client = Mqtt5Client.builder()
                .identifier(UUID.randomUUID().toString())
                .serverHost("mqtt.flespi.io")
                .simpleAuth(Mqtt5SimpleAuth.builder().username("3s897csODyMGcSwQ75LY7uTipFBIBnzsDvrBvHfZ6Pt6xQMsLnhGH0PVvetUrQcU").build())
                .buildBlocking();

        client.connect();

        client.publishWith()
                .topic("cyber/rssi")
                .qos(MqttQos.AT_LEAST_ONCE)
                .payload("payload ciao".getBytes())
                .send();
//        try (final Mqtt5BlockingClient.Mqtt5Publishes publishes = client.publishes(MqttGlobalPublishFilter.ALL)) {
//
//            client.subscribeWith().topicFilter("cyber/rssi").qos(MqttQos.AT_LEAST_ONCE).send();
//
//            publishes.receive(1, TimeUnit.SECONDS).ifPresent(System.out::println);
//            publishes.receive(100, TimeUnit.MILLISECONDS).ifPresent(System.out::println);
//
//        } finally {
//            client.disconnect();
//        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onSupportNavigateUp() {
        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment);
        return NavigationUI.navigateUp(navController, mAppBarConfiguration)
                || super.onSupportNavigateUp();
    }

    BeaconManager beaconManager = new BeaconManager();

    class BeaconManager {

        ArrayList<Beacon> beacons = new ArrayList<>();
        String BEN = "d5:61:6b:fb:8d:e3";
        String CARL = "d6:82:a5:47:bf:ac";
        String DAVE = "c5:7c:30:e4:a5:66";
        String FERRANTE = "f7:a2:7a:d2:40:1c";
        String SIMO = "f3:4e:e8:df:11:bc";
        List<String> addresses = Arrays.asList(FERRANTE, BEN, SIMO);
        //"d5:61:6b:fb:8d:e3");
//        ,
//                "d6:82:a5:47:bf:ac",
//                "c5:7c:30:e4:a5:66");

        void connectBeacons() {
            beacons.clear();
            // Spawn a new thread to avoid blocking the GUI one

            Thread t = new Thread() {
                public void run() {
                    internalRun();

                }
            };
            t.setDaemon(true);
            t.start();
        }

        private void internalRun() {


            addresses.forEach(addr -> {
                String address = addr.toUpperCase();
                BluetoothDevice device = mBTAdapter.getRemoteDevice(address);
                Beacon beacon = new Beacon(addr);
                BluetoothGatt gatt = device.connectGatt(getApplicationContext(), true, beacon);
                beacon.gatt = gatt;
                beacon.device = device;
                beacons.add(beacon);
                try {
                    Thread.sleep(900);
                } catch (InterruptedException e) {

                }

            });
            int index = 0;
            long prev = System.currentTimeMillis();
            while (Thread.currentThread().isAlive()) {

                long curr = System.currentTimeMillis();

                index++;
                if (index >= beacons.size())
                    index = 0;

                beacons.get(index).gatt.readRemoteRssi();
                StringBuilder sb = new StringBuilder();
                beacons.forEach(b -> {
                    sb.append(b.rssi);
                    sb.append(",");
                });

                Log.i("CYBER", "rssi " + sb.toString());
//                Log.i("CYBER", "looping");
                try {
                    Thread.sleep(90);
                } catch (InterruptedException e) {

                }
            }
        }
    }

    class Beacon extends BluetoothGattCallback {
        private String address;
        private BluetoothDevice device;
        private BluetoothGatt gatt;
        long counter = 0;
        int rssi = 0;

        public Beacon(String address) {
            this.address = address;
//            this.device = device;
        }


        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            super.onConnectionStateChange(gatt, status, newState);
            if (newState == BluetoothProfile.STATE_CONNECTED) {
                Log.i("CYBER", gatt.getDevice().getAddress() + " " + address + " connected");

                gatt.readRemoteRssi();
            } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                Log.i("CYBER", address + " disconnected");
            }
        }

        @Override
        public void onReadRemoteRssi(BluetoothGatt gatt, int rssi, int status) {
            super.onReadRemoteRssi(gatt, rssi, status);
            counter++;
//            if (counter % 100 == 0)
            //Log.i("CYBER", gatt.getDevice().getAddress() + " " + this.gatt.getDevice().getAddress() + " " + address + " rssi " + rssi);
//            this.gatt.readRemoteRssi();
            this.rssi = rssi;
        }
    }
}
