package com.example.clientsocketudp_android2;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.app.Activity;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnLongClickListener;
import android.view.View.OnTouchListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;



public class MainActivity extends Activity {

    EditText editTextAddress, editTextPort;
    TextView outputDebug;
    Button buttonConnect, cmdA, cmdR, cmdG, cmdD, cmdUp, cmdDown;
    Socket socket;
    //BufferedReader in;
   BufferedWriter out;    
    

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        editTextAddress = (EditText)findViewById(R.id.address);
        editTextPort = (EditText)findViewById(R.id.port);
        outputDebug = (TextView)findViewById(R.id.outputDebug);
        
        buttonConnect = (Button)findViewById(R.id.connect);
        
        cmdA = (Button)findViewById(R.id.cmdA);
        cmdR = (Button)findViewById(R.id.cmdR);
        cmdD = (Button)findViewById(R.id.cmdD);
        cmdG = (Button)findViewById(R.id.cmdG);
        cmdUp = (Button)findViewById(R.id.cmdUp);
        cmdDown = (Button)findViewById(R.id.cmdDown);


        buttonConnect.setOnClickListener(buttonConnectOnClickListener);

        cmdA.setOnTouchListener(cmdAOnLongClickListener);
        cmdR.setOnLongClickListener(cmdROnLongClickListener);
        cmdD.setOnLongClickListener(cmdDOnLongClickListener);
        cmdG.setOnLongClickListener(cmdGOnLongClickListener);
        cmdUp.setOnLongClickListener(cmdUpOnLongClickListener);
        cmdDown.setOnLongClickListener(cmdDownOnLongClickListener);
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
		 
		private Handler mHandler;
		
		Runnable mAction = new Runnable() {			
			@Override
			public void run()
			{
				// TODO Auto-generated method stub
				//outputDebug.append("a");
				try {
					int i = 0;
					while(true) 
					{
						i++;
						// buffer client : 8192 octets
						// buffer serveur : 16384 octets
						if(i==16384) 
						{
							out.write('a');
						}
						out.write('a');
					}
					//outputDebug.append("a");
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				mHandler.postDelayed(mAction, 50);
			}
		};
		
		@Override
		public boolean onTouch(View v, MotionEvent event) {
			if(event.getAction()==MotionEvent.ACTION_DOWN)
			{
				if(mHandler!=null ) return true;
				mHandler = new Handler();
				mHandler.postDelayed(mAction, 50);
			}
			else if (event.getAction()==MotionEvent.ACTION_UP)
			{
				if(mHandler == null) return true;
				mHandler.removeCallbacks(mAction);
				mHandler=null;
			}
			return false;
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
		
		OnLongClickListener cmdUpOnLongClickListener = new OnLongClickListener() 
		{					
			@Override
			public boolean onLongClick(View v) 
			{
					try {
						out.write('+');
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				
				// TODO Auto-generated method stub
				return false;
			}
		};
		
		OnLongClickListener cmdDownOnLongClickListener = new OnLongClickListener() 
		{					
			@Override
			public boolean onLongClick(View v) 
			{
					try {
						out.write('+');
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
			return null;
        }        
    }

}