package com.lge.fcc.like;

import com.lge.fcc.like.json.Join;
import com.lge.fcc.like.json.Leave;

import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.Menu;

public class LikeActivity extends Activity {
	private LikeTransaction likeTransaction;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_like);
		
		likeTransaction = LikeTransaction.connect("10.177.17.41", 8181);
		if (likeTransaction != null) {
			Log.e("SOZU", likeTransaction.transact(Join.to("AAA", "apple")));
		} else {
			Log.e("SOZU", "connection failed!");
		}
	}
	
	@Override
	protected void onDestroy() {
		super.onDestroy();
		
		if (likeTransaction != null) {
			likeTransaction.transact(Leave.to());
			likeTransaction.disconnect();
		}
		likeTransaction = null;
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.like, menu);		
		return true;
	}

}
