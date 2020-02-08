#include <jni.h>
#include <string>
#include <ctime>
#include "httplib.h"
#include "android/log.h"

#define HTTP_OK 200

httplib::Params retrieveParameters(JNIEnv *, jobject);

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_anative_MainActivity_cpp_1get_1req(JNIEnv *env, jobject thiz, jobject params) {
    httplib::Client client("httpbin.org");

    httplib::Params parameters = retrieveParameters(env, params);
    std::string query = "?";
    for (auto cur:parameters) {
        query += cur.first;
        query += "=";
        query += cur.second;
        query += "&";
    }
    query.pop_back();
    std::string path = "/get" + query;

    __android_log_write(ANDROID_LOG_INFO, "Android Native Log Tag", ("url: "+path).c_str());

    auto response = client.Get(path.c_str());

    std::string respstr;
    if (response && response->status == HTTP_OK) {
        respstr = response->body;
    } else {
        respstr = "Something went wrong!";
    }
    return env->NewStringUTF(respstr.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_anative_MainActivity_cpp_1post_1req(JNIEnv *env, jobject thiz, jobject params) {
    httplib::Client client("httpbin.org");

    httplib::Params parameters = retrieveParameters(env, params);

    auto response = client.Post("/post", parameters);

    std::string respstr;
    if (response && response->status == HTTP_OK) {
        respstr = response->body;
    } else {
        respstr = "Something went wrong!";
    }
    return env->NewStringUTF(respstr.c_str());
}

httplib::Params retrieveParameters(JNIEnv *env, jobject params) {
    jclass listClass = env->GetObjectClass(params);
    jmethodID sizeMethodsID = env->GetMethodID(listClass, "size", "()I");
    jmethodID getMethodsID = env->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");

    jint sz = env->CallIntMethod(params, sizeMethodsID);

    httplib::Params parameters;
    for (jint i = 0; i < sz; i++) {
        jobject p = env->CallObjectMethod(params, getMethodsID, i);
        jclass pairClass = env->GetObjectClass(p);
        jfieldID firstField = env->GetFieldID(pairClass, "first", "Ljava/lang/Object;");
        jfieldID secondField = env->GetFieldID(pairClass, "second", "Ljava/lang/Object;");
        jstring first = (jstring) env->GetObjectField(p, firstField);
        jstring second = (jstring) env->GetObjectField(p, secondField);
        jboolean bf, sf;
        parameters.insert(
                {env->GetStringUTFChars(first, &bf), env->GetStringUTFChars(second, &sf)});
    }
    return parameters;
}