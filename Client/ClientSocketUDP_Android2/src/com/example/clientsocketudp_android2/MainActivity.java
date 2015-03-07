package com.example.clientsocketudp_android2;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
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
    TextView outputDebug, speed, distance;
    Button buttonConnect, cmdA, cmdR, cmdG, cmdD, cmdUp, cmdDown;
    Socket socket;
    //BufferedReader in;
    DataInputStream r;
   BufferedWriter out;
   int i = 5;
   int distanceF;
   
    

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        editTextAddress = (EditText)findViewById(R.id.address);
        editTextPort = (EditText)findViewById(R.id.port);
        outputDebug = (TextView)findViewById(R.id.outputDebug);
        speed = (TextView)findViewById(R.id.speed);
        distance = (TextView)findViewById(R.id.distance);
        
        buttonConnect = (Button)findViewById(R.id.connect);
        
        cmdA = (Button)findViewById(R.id.cmdA);
        cmdR = (Button)findViewById(R.id.cmdR);
        cmdD = (Button)findViewById(R.id.cmdD);
        cmdG = (Button)findViewById(R.id.cmdG);
        cmdUp = (Button)findViewById(R.id.cmdUp);
        cmdDown = (Button)findViewById(R.id.cmdDown);


        buttonConnect.setOnClickListener(buttonConnectOnClickListener);

        cmdA.setOnTouchListener(cmdAOnLongClickListener);
        cmdR.setOnTouchListener(cmdROnLongClickListener);
        cmdD.setOnTouchListener(cmdDOnLongClickListener);
        cmdG.setOnTouchListener(cmdGOnLongClickListener);
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
				try {
					out.write('a');
	    			out.flush();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				outputDebug.append("a");

				mHandler.post(mAction);
			}
		};
		
		@Override
		public boolean onTouch(View v, MotionEvent event) {
			if(event.getAction()==MotionEvent.ACTION_DOWN)
			{
				try {
	                r.readInt();
					out.write('a');
					out.flush();
					outputDebug.append("a");


				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
    			
			}
			else if (event.getAction()==MotionEvent.ACTION_UP)
			{

				try {
					out.write('s');
					out.flush();
					outputDebug.append("s");
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
    			
			}
			return false;
		}
	};
	   
	 OnTouchListener cmdROnLongClickListener = new OnTouchListener() 
	 {
			private Handler mHandler;
			
			Runnable mAction = new Runnable() 
			{			
				@Override
				public void run()
				{
					try 
					{
						out.write('r');
		    			out.flush();
					} 
					catch (IOException e) 
					{
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					outputDebug.append("r");

					mHandler.post(mAction);
				}
			};
			
		
		@Override
		public boolean onTouch(View v, MotionEvent event) {
			if(event.getAction()==MotionEvent.ACTION_DOWN)
			{
				try {
					out.write('r');
					out.flush();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
    	
				
			}

			else if (event.getAction()==MotionEvent.ACTION_UP)
			{
				try {
					out.write('s');
					out.flush();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
    			
			}
			// TODO Auto-generated method stub
			return false;
		}
	};

	 OnTouchListener cmdDOnLongClickListener = new OnTouchListener() {
		 private Handler mHandler;
			
			Runnable mAction = new Runnable() 
			{			
				@Override
				public void run()
				{
					try 
					{
						out.write('d');
		    			out.flush();
					} 
					catch (IOException e) 
					{
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					outputDebug.append("d");

					mHandler.post(mAction);
				}
			};
		
		@Override
		public boolean onTouch(View v, MotionEvent event) {
			if(event.getAction()==MotionEvent.ACTION_DOWN)
			{
				try {
					out.write('d');
	    			out.flush();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
	
			}
			else if (event.getAction()==MotionEvent.ACTION_UP)
			{
				try {
					out.write('s');
	    			out.flush();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}

			}
			// TODO Auto-generated method stub
			return false;
		}
	}; 

			 
		OnTouchListener cmdGOnLongClickListener = new OnTouchListener() {
			 private Handler mHandler;
				
				Runnable mAction = new Runnable() 
				{			
					@Override
					public void run()
					{
						try 
						{
							out.write('g');
			    			out.flush();
						} 
						catch (IOException e) 
						{
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
						outputDebug.append("g");

						mHandler.post(mAction);
					}
				};
			
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				if(event.getAction()==MotionEvent.ACTION_DOWN)
				{
					try 
					{
						out.write('g');
		    			out.flush();
					} 
					catch (IOException e) 
					{
						// TODO Auto-generated catch block
						e.printStackTrace();
					}

				}
				else if (event.getAction()==MotionEvent.ACTION_UP)
				{
					try 
					{
						out.write('s');
		    			out.flush();
					} 
					catch (IOException e) 
					{
						// TODO Auto-generated catch block
						e.printStackTrace();
					}

				}
				// TODO Auto-generated method stub
				return false;
			}
		};

		
		OnLongClickListener cmdUpOnLongClickListener = new OnLongClickListener() {
			
			@Override
			public boolean onLongClick(View v) {
				
				try {
					//int i = 5;
					out.write('+');
					out.flush();
					if(i<10) i = i+1;
					
					//speed.setText(2);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				outputDebug.append("+");
				speed.setText("" +i);
    			
				// TODO Auto-generated method stub
				return false;
			}
		};
			


		
		OnLongClickListener cmdDownOnLongClickListener = new OnLongClickListener() {
			
			@Override
			public boolean onLongClick(View v) {
				try {
					out.write('-');
					out.flush();
					if(i>0) i = i-1;
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				outputDebug.append("-");
				speed.setText("" +i);
			
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
        		r = new DataInputStream(socket.getInputStream());
        		out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()), 1);

        		//r.readInt();
        		//distance.setText("" +1);


            } catch (UnknownHostException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
			return null;
        }

		@Override
		protected void onPostExecute(Void result) {
			// TODO Auto-generated method stub
			super.onPostExecute(result);
		}
        
        
    }

}