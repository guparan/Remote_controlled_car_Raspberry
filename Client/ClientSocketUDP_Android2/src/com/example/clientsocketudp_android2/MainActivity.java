package com.example.clientsocketudp_android2;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.concurrent.TimeUnit;

import android.os.AsyncTask;
import android.os.Bundle;
import android.app.Activity;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends Activity {

    TextView textResponse, textPerformance;
    EditText editTextAddress, editTextPort;
    Button buttonConnect, buttonClear, buttonOK;
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
        buttonClear = (Button)findViewById(R.id.clear);
        buttonOK = (Button)findViewById(R.id.OK);
        textResponse = (TextView)findViewById(R.id.response);
        textPerformance = (TextView)findViewById(R.id.Performance); 
        //textPerformance.setText("Hello");

        buttonConnect.setOnClickListener(buttonConnectOnClickListener);
        buttonClear.setOnClickListener(new OnClickListener()
        {
            @Override
            public void onClick(View v) {
                textResponse.setText("");
            }});
        
        buttonOK.setOnClickListener( 
        	new OnClickListener() 
	        {
	            @Override
	            public void onClick(View v) 
	            {	                
	                //textPerformance.setText( String.valueOf(message_distant) );
	            }
	        }
        );
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
    		
    		while(true)
    		{
    			try {
					out.write('a');
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
					              
          
				
                
    		}
        }
        
        String asciiValuesOf(char[] text, int start, int length)
        {
			 String asciiValues = new String();
			 
			 for(int i=start ; i<start+length ; i++)
			 {
				 asciiValues = asciiValues.concat( String.valueOf((int)text[i]) );
			 }
				
			 return asciiValues;
        }
        
        

        @Override
        protected void onPostExecute(Void result) {
            textResponse.setText(response);
            super.onPostExecute(result);
        }

    }

}