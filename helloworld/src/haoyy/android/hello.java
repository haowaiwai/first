package haoyy.android;

import android.app.Activity;
import android.os.Bundle;

public class hello extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
    	//	向导生成
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
    }
}