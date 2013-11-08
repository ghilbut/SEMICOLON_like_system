package com.lge.fcc.like;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;

public class IntroActivity extends Activity {
	private Bitmap loading;
	private ImageView imageView;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
		setContentView(R.layout.activity_intro);
		
		loading = BitmapFactory.decodeStream(getResources().openRawResource(R.drawable.loading));
		imageView = (ImageView)findViewById(R.id.logoImageView);
		imageView.setImageBitmap(loading);
		
		new Thread(new Runnable() {
			@Override
			public void run() {
				ImageView logo = (ImageView)findViewById(R.id.logoImageView);
				Animation animFrom = AnimationUtils.loadAnimation(IntroActivity.this, R.anim.alpha);
				logo.startAnimation(animFrom);
				try {
					Thread.sleep(4000);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				new Handler(Looper.getMainLooper()).post(new Runnable() {
					@Override
					public void run() {
						if (imageView != null) {
							imageView.setImageBitmap(null);
							imageView = null;
						}
						if (loading != null) {
							loading.recycle();
							loading = null;
						}
						Intent intent = new Intent(IntroActivity.this, LikeActivity.class);
						intent.setFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);
						startActivity(intent);
						finish();
					}
				});
				
			}
		}).start();
	}
}
