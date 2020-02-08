package com.example.anative;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.os.Bundle;
import android.util.Pair;
import android.view.ContextMenu;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.GregorianCalendar;
import java.util.List;
import java.util.Objects;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private Button getBtn;
    private Button postBtn;
    private EditText response_arena;
    private EditText value1, value2, value3;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        value1 = findViewById(R.id.editText);
        value2 = findViewById(R.id.editText2);
        value3 = findViewById(R.id.editText3);

        getBtn = findViewById(R.id.get_btn);
        postBtn = findViewById(R.id.post_btn);

        response_arena = findViewById(R.id.resp_area);

        getBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                List<Pair<String, String>> params = new ArrayList<>();
                params.add(new Pair<>("param1", value1.getText().toString()));
                params.add(new Pair<>("param2", value2.getText().toString()));
                params.add(new Pair<>("param3", value3.getText().toString()));
                response_arena.setText(cpp_get_req(params));

                ((InputMethodManager) Objects.requireNonNull(getSystemService(Context.INPUT_METHOD_SERVICE))).hideSoftInputFromWindow(response_arena.getWindowToken(), 0);
            }
        });
        postBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                List<Pair<String, String>> params = new ArrayList<>();
                params.add(new Pair<>("param1", value1.getText().toString()));
                params.add(new Pair<>("param2", value2.getText().toString()));
                params.add(new Pair<>("param3", value3.getText().toString()));
                response_arena.setText(cpp_post_req(params));

                ((InputMethodManager) Objects.requireNonNull(getSystemService(Context.INPUT_METHOD_SERVICE))).hideSoftInputFromWindow(response_arena.getWindowToken(), 0);
            }
        });
    }

    /**
     * native methods
     */
    public native String cpp_get_req(List<Pair<String, String>> params);

    public native String cpp_post_req(List<Pair<String, String>> params);
}
