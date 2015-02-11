package com.example.clientsocketudp_android2;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import android.os.AsyncTask;
import android.os.Bundle;
import android.app.Activity;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnLongClickListener;
import android.view.View.OnTouchListener;
import android.widget.Button;
import android.widget.EditText;



public class MainActivity extends Activity {

    EditText editTextAddress, editTextPort;
    Button buttonConnect, cmdA, cmdR, cmdG, cmdD;
    Socket socket;
    //BufferedReader in;
   BufferedWriter out;    
    

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        editTextAddress = (EditText)findViewById(R.id.address);
        editTextPort = (EditText)findViewById(R.id.port);
        
        buttonConnect = (Button)findViewById(R.id.connect);
        
        cmdA = (Button)findViewById(R.id.cmdA);
        cmdR = (Button)findViewById(R.id.cmdR);
        cmdD = (Button)findViewById(R.id.cmdD);
        cmdG = (Button)findViewById(R.id.cmdG);

        buttonConnect.setOnClickListener(buttonConnectOnClickListener);

        cmdA.setOnTouchListener(cmdAOnLongClickListener);
        cmdR.setOnLongClickListener(cmdROnLongClickListener);
        cmdD.setOnLongClickListener(cmdDOnLongClickListener);
        cmdG.setOnLongClickListener(cmdGOnLongClickListener);
     }

    OnClickListener buttonConnectOnClickListener = new OnClickListener()
    {
        @Override
        public void onClick(View arg0) 
        {
        	MyClientTask myClientTask = new MyClientTask();
            myClientTask.execute();
        }
	};

	 OnTouchListener cmdAOnLongClickListener = new OnTouchListener() {
		
		@Override
		public boolean onTouch(View v, MotionEvent event) {
			try {
				out.write('a');
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			return true;
		}
	};
	   
	 OnLongClickListener cmdROnLongClickListener = new OnLongClickListener()
	 {			
			@Override
			public boolean onLongClick(View v) 
			{
					try {
						out.write('r');
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				
				// TODO Auto-generated method stub
				return false;
			}
		};
		
	 OnLongClickListener cmdDOnLongClickListener = new OnLongClickListener() 
	 {
			@Override
			public boolean onLongClick(View v) 
			{				
					try {
						out.write('d');
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				
				// TODO Auto-generated method stub
				return false;
			}
		};
			 
		OnLongClickListener cmdGOnLongClickListener = new OnLongClickListener() 
		{					
			@Override
			public boolean onLongClick(View v) 
			{
					try {
						out.write('g');
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				
				// TODO Auto-generated method stub
				return false;
			}
		};
		
    public class MyClientTask extends AsyncTask<Void, Void, Void> {

        String dstAddress;
        int dstPort;
        String response;

        @Override
        protected Void doInBackground(Void... arg0) 
        {
        	try 
        	{
        		socket = new Socket(editTextAddress.getText().toString(), Integer.parseInt(editTextPort.getText().toString()) );
        		//in = new BufferedReader (new InputStreamReader (socket.getInputStream()));
        		out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
            } catch (UnknownHostException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        	
        	try {
				out.write('c');
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
        	
			return null;
        }        
    }

}