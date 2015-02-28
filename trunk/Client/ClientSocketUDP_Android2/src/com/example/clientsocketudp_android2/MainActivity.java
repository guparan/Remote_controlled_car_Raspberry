package com.example.clientsocketudp_android2;

import java.io.BufferedWriter;
import java.io.IOException;
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
        cmdR.setOnTouchListener(cmdROnLongClickListener);
        cmdD.setOnTouchListener(cmdDOnLongClickListener);
        cmdG.setOnTouchListener(cmdGOnLongClickListener);
        cmdUp.setOnTouchListener(cmdUpOnLongClickListener);
        cmdDown.setOnTouchListener(cmdDownOnLongClickListener);
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
				if(mHandler!=null ) return true;
				mHandler = new Handler();
				mHandler.post(mAction);
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
				if(mHandler!=null ) return true;
				mHandler = new Handler();
				mHandler.post(mAction);
			}
			else if (event.getAction()==MotionEvent.ACTION_UP)
			{
				if(mHandler == null) return true;
				mHandler.removeCallbacks(mAction);
				mHandler=null;
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
					outputDebug.append("r");

					mHandler.post(mAction);
				}
			};
		
		@Override
		public boolean onTouch(View v, MotionEvent event) {
			if(event.getAction()==MotionEvent.ACTION_DOWN)
			{
				if(mHandler!=null ) return true;
				mHandler = new Handler();
				mHandler.post(mAction);
			}
			else if (event.getAction()==MotionEvent.ACTION_UP)
			{
				if(mHandler == null) return true;
				mHandler.removeCallbacks(mAction);
				mHandler=null;
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
						outputDebug.append("r");

						mHandler.post(mAction);
					}
				};
			
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				if(event.getAction()==MotionEvent.ACTION_DOWN)
				{
					if(mHandler!=null ) return true;
					mHandler = new Handler();
					mHandler.post(mAction);
				}
				else if (event.getAction()==MotionEvent.ACTION_UP)
				{
					if(mHandler == null) return true;
					mHandler.removeCallbacks(mAction);
					mHandler=null;
				}
				// TODO Auto-generated method stub
				return false;
			}
		};

		
		OnTouchListener cmdUpOnLongClickListener = new OnTouchListener() {
			
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub
				return false;
			}
		};

		
		OnTouchListener cmdDownOnLongClickListener = new OnTouchListener() {
			
			@Override
			public boolean onTouch(View v, MotionEvent event) {
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
        		out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()), 1);        		
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