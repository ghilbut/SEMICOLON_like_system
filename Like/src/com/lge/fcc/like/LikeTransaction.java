package com.lge.fcc.like;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

import android.util.Log;

public class LikeTransaction {	
	private Socket socket;
	private BufferedReader in;
	private PrintWriter out;
	private ExecutorService executor;
	
	private LikeTransaction() {	}
	public static LikeTransaction connect(final String ip, final int port) {
        final LikeTransaction likeTransaction = new LikeTransaction();
        likeTransaction.executor = Executors.newSingleThreadExecutor();
        try {
			likeTransaction.executor.submit(new Runnable() {
				@Override
				public void run() {
					try {
			        	likeTransaction.socket = new Socket(ip, port);
					} catch (IOException e) {
						likeTransaction.socket = null;
						e.printStackTrace();
					}
				}       	
			}).get();
		} catch (InterruptedException e) {
			e.printStackTrace();
			return null;
		} catch (ExecutionException e) {
			e.printStackTrace();
			return null;
		}
        if (likeTransaction.socket == null) {
        	return null;
        }
        try {
        	likeTransaction.in = new BufferedReader(new InputStreamReader(likeTransaction.socket.getInputStream()));
			likeTransaction.out = new PrintWriter(likeTransaction.socket.getOutputStream(), true);
		} catch (IOException e) {
			e.printStackTrace();
			likeTransaction.disconnect();
			return null;
		}
        return likeTransaction;
	}
	public String transact(final String message) {
		Log.e("SOZU", "transact - " + message);
		final StringBuffer buffer = new StringBuffer();
		
		try {
			executor.submit(new Runnable() {
				@Override
				public void run() {
					out.println(String.format("%4d", message.length()) + message);
					try {
						buffer.append(in.readLine());
					} catch (IOException e) {
						e.printStackTrace();
					}
				}		
			}).get(10000, TimeUnit.MILLISECONDS);
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (ExecutionException e) {
			e.printStackTrace();
		} catch (TimeoutException e) {
			e.printStackTrace();
			Log.d("SOZU", "time out...");
		}	
		return buffer.toString();
	}
	public void disconnect() {
		try {
			if (in != null) {
				in.close();
			}
			if (out != null) {
				out.close(); 
			}
			if (socket != null && socket.isConnected()) {
				socket.close();
			
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		in = null;
		out = null;
		socket = null;
	}
}
