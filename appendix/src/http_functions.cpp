
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <curl/curl.h>
using namespace std;

#define BUFFER_SIZE  (256 * 1024)  /* 256 KB */

#define URL_FORMAT   "https://wodinaz.com/%s"
#define URL_SIZE     256

struct write_result
{
    char *data;
    int pos;
};

static size_t write_response(void *ptr, size_t size, size_t nmemb, void *stream)
{
    struct write_result *result = (struct write_result *)stream;

    if(result->pos + size * nmemb >= BUFFER_SIZE - 1)
    {
        fprintf(stderr, "error: too small buffer\n");
        return 0;
    }

    memcpy(result->data + result->pos, ptr, size * nmemb);
    result->pos += size * nmemb;

    return size * nmemb;
}

// make HTTP request to url
char* http_request(char *url)
{
    CURL *curl = NULL;
    CURLcode status;
    struct curl_slist *headers = NULL;
    char *data = NULL;
    long code;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(!curl)
        goto error;

    data = (char*)malloc(BUFFER_SIZE);
    if(!data)
        goto error;

    struct write_result write_result;
    write_result.data=data;
    write_result.pos=0;

    curl_easy_setopt(curl, CURLOPT_URL, url);

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &write_result);

    status = curl_easy_perform(curl);
    if(status != 0)
    {
        fprintf(stderr, "error: unable to request data from %s:\n", url);
        fprintf(stderr, "%s\n", curl_easy_strerror(status));
        goto error;
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
    if(code != 200)
    {
        fprintf(stderr, "error: server responded with code %ld\n", code);
        goto error;
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    curl_global_cleanup();

    /* zero-terminate the result */
    data[write_result.pos] = '\0';

    return data;

error:
    if(data)
        free(data);
    if(curl)
        curl_easy_cleanup(curl);
    if(headers)
        curl_slist_free_all(headers);
    curl_global_cleanup();
    return NULL;
}

//post to server
void http_post(char* url, char* json_string){
     CURL *curl;
     CURLcode res;
     
      /* In windows, this will init the winsock stuff */ 
      curl_global_init(CURL_GLOBAL_ALL);
      /* get a curl handle */ 
      curl = curl_easy_init();
      if(curl) {
        /* First set the URL that is about to receive our POST. This URL can
           just as well be a https:// URL if that is what should receive the
           data. */ 
        curl_easy_setopt(curl, CURLOPT_URL, url);

        /* Now specify the POST data */ 
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_string);

        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);

        /* Check for errors */ 
        if(res != CURLE_OK)
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));
     

        //printf("return code:%d\n",res );
        /* always cleanup */ 
        curl_easy_cleanup(curl);
      }
      curl_global_cleanup();
}
