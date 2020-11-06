package com.teenenggr.esp8266_wifi_ledcontrol;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.Switch;
import java.io.*;
import java.net.*;
import java.nio.charset.Charset;

public class MainActivity extends AppCompatActivity {

    Socket echoSocket = null;
    DataOutputStream os = null;
    DataInputStream is = null;
    DataInputStream stdIn = new DataInputStream(System.in);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        try {
            echoSocket = new Socket("192.168.4.1", 80);
            os = new DataOutputStream(echoSocket.getOutputStream());
            is = new DataInputStream(echoSocket.getInputStream());

        } catch (UnknownHostException e) {
            System.err.println("Don't know about host: Jatin");
        } catch (IOException e) {
            System.err.println("Couldn't get I/O for the connection to: Jatin");
        }

        final Switch ledSwitch = (Switch)findViewById(R.id.led_switch);

        ledSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                try {
                    if (b) {
                        os.write("pin=6".getBytes(Charset.forName("UTF-8")));
                    } else {
                        os.write("pin=7".getBytes(Charset.forName("UTF-8")));
                    }
                } catch (Exception e) {
                    System.err.println("Can't send the data");
                }
            }
        });


    }
}
