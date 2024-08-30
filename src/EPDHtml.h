/*#pragma once
const String NotFoundHtml=
"<html>\
    <head>\
    </head>\
    <body>\
      404 NOT FOUND\
    </body>\
    </html>";

const String HomeHtml =
    "<html>\
    <head>\
       <meta charset=\"utf-8\"> \
       <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
        <title>EPD Editor</title>\
    </head>\
    <body>\
    <a href=\"query\"><input type=\"button\" value=\"查询\"></a>\
    <a href=\"remove\"><input type=\"button\" value=\"移除\"></a><br>\
    <form action="">\
        <table>\
            <tr>\
                <td>日期：</td>\
                <td><input type=\"date\" id=\"start_date\" name=\"date\"><br><input type=\"date\" id=\"end_date\" name=\"date\"></td>\
            </tr>\
            <tr>\
                <td>模式：</td>\
                <td>\
                    <select name=\"choice\" id=\"tool_select\" value=\"Tool:\">\
                        <option value=\"week\">周</option>\
                        <option value=\"day\">日</option>\
                    </select>\
                </td>\
            </tr>\
            <tr>\
                <td>值：</td>\
                <td><input type=\"number\" id=\"value_number\"  min = \"1\" step = \"1\" value = \"1\"></td>\
            </tr>\
            <tr>\
                <td>事项：</td>\
                <td><input type=\"text\" value=\"\" id=\"todo_text\" ></td>\
                <!--onchange=\"Update()\"-->\
            </tr>\
            <tr>\
                <td><input type=\"button\" value=\"更新\" id=\"update_button\" onclick=\"Update()\"></td>\
                <td><input type=\"button\" value=\"刷新\" id=\"refresh_button\" onclick=\"Refresh()\"></td>\
                <td><input type=\"button\" value=\"关机\" id=\"sleep_button\" onclick=\"EPDSleep()\"></td>\
            </tr>\
        </table>\
    </form>\
                <td><div id=\"message_text\">无</div></td>\
</body>\
        <script defer=\"defer\">\
            function Update() {\
                var xmlhttp;\
                var d1=start_date.value;\
                var d2=end_date.value;\
                var v=value_number.value;\
                var tt = todo_text.value;\
                var tool=tool_select.value;\
                if(tool==\" \")return;\
                if(d2==\"\")d2=d1;\
                if(tt==\"\"||v==\"\")return;\
                if (window.XMLHttpRequest) {\
                    xmlhttp = new XMLHttpRequest();\
                } else {\
                    xmlhttp = new ActiveXObject(\"Microsoft.XMLHTTP\");\
                }\
                xmlhttp.onreadystatechange = function () {\
                    if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {\
                      document.getElementById(\"message_text\").innerHTML = xmlhttp.responseText;\
                    }\
                };\
                xmlhttp.open(\"GET\", \"update?todo=\"+tt+\"&startdate=\"+d1+\"&enddate=\"+d2+\"&value=\"+v+\"&tool=\"+tool, true);\
                xmlhttp.send();\
            }\
            function Refresh(){\
                var xmlhttp;\
                if (window.XMLHttpRequest) {\
                    xmlhttp = new XMLHttpRequest();\
                } else {\
                    xmlhttp = new ActiveXObject(\"Microsoft.XMLHTTP\");\
                }\
                xmlhttp.open(\"GET\", \"refresh\", true);\
                xmlhttp.send();\
            }\
            function EPDSleep(){\
                var xmlhttp;\
                if (window.XMLHttpRequest) {\
                    xmlhttp = new XMLHttpRequest();\
                } else {\
                    xmlhttp = new ActiveXObject(\"Microsoft.XMLHTTP\");\
                }\
                xmlhttp.open(\"GET\", \"sleep\", true);\
                xmlhttp.send();\
            }\
        </script>\
</html>";
//------------------------------------------------------------------------------------------------------------------------------
const String QueryPage=
"<html>\
    <head>\
       <meta charset=\"utf-8\">\
       <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
        <title>EPD Editor-Query</title>\
    </head>\
    <body>\
    <a href=\"../\"><input type=\"button\" value=\"主页\"></a>\
    <a href=\"../remove\"><input type=\"button\" value=\"移除\"></a><br>\
    <form action="">\
        <table>\
            <tr>\
                <td>查询日期：</td>\
                <td><input type=\"date\" id=\"query_date\" name=\"date\"></td>\
                <td><input type=\"button\" value=\"查询\" id=\"query_button\" onclick=\"QueryDate()\"></td>\
            </tr>\
            <tr>\
                <td>事项列表：</td>\
                <td>\
                        <ul id=\"todo_list\">\
                        </ul>\
                </td>\
            </tr>\
        </table>\
    </form>\
   <td><div id=\"message_text\">无</div></td>\
</body>\
    <script defer=\"defer\">\
        function addItem(str) {\
            var list = document.getElementById('todo_list');\
            var newItem = document.createElement('li');\
            newItem.textContent = str;\
            list.appendChild(newItem);\
        }\
        function removeItem(i) {\
            var index=Number(i);\
            if(index){\
                var list = document.getElementById('todo_list');\
                var listItems = list.getElementsByTagName('li');\
                if (list.hasChildNodes()&&listItems.length>=i) {\
                    list.removeChild(list.childNodes[2*index-1]);\
                }\
            }\
        }\
        function QueryDate(){\
                var xmlhttp;\
                var dd=query_date.value;\
                if (window.XMLHttpRequest) {\
                    xmlhttp = new XMLHttpRequest();\
                } else {\
                    xmlhttp = new ActiveXObject(\"Microsoft.XMLHTTP\");\
                }\
                xmlhttp.onreadystatechange = function () {\
                    if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {\
                      var array=xmlhttp.responseText.split(\" \");\
                      var len=array.length;\
                      document.getElementById('todo_list').innerHTML=\"\";\
                      if(len==0)return;\
                      for(i=0;i<len-1;i++){\
                      addItem(array[i]);}\
                    };\
                };\
                xmlhttp.open(\"GET\", \"querydate?date=\"+dd, true);\
                xmlhttp.send();\
        }\
    </script>\
</html>";
const String RemoveHtml=
"<html>\
    <head>\
       <meta charset=\"utf-8\">\
       <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
        <title>EPD Editor-Query</title>\
    </head>\
    <body>\
    <a href=\"../query\"><input type=\"button\" value=\"查询\"></a>\
    <a href=\"../\"><input type=\"button\" value=\"主页\"></a><br>\
    <form action=\"\">\
        <table>\
            <tr>\
                <td>查询：</td>\
                <td><input type=\"button\" value=\"查询\" id=\"query_button\" onclick=\"QueryDateAll()\"></td>\
            </tr>\
            <tr>\
                <td>所有事项:</td>\
                <td>\
                        <ul id=\"todo_list\">\
                        </ul>\
                </td>\
            </tr>\
            <tr>\
                <td>移除:</td>\
                <td><input type=\"number\" min = \"1\" max = \"6\" step = \"1\" value = \"1\" id=\"remove_number\"><td>\
                <td><input type=\"button\" value=\"移除\" id=\"remove_button\" onclick=\"clickRemoveButton()\"></td>\
            </tr>\
        </table>\
    </form>\
     <td><div id=\"message_text\">无</div></td>\
</body>\
    <script defer=\"defer\">\
        function addItem(str) {\
            var list = document.getElementById('todo_list');\
            var newItem = document.createElement('li');\
            newItem.textContent = str;\
            list.appendChild(newItem);\
        }\
        function postRemoveItem(str){\
                var xmlhttp;\
                if (window.XMLHttpRequest) {\
                    xmlhttp = new XMLHttpRequest();\
                } else {\
                    xmlhttp = new ActiveXObject(\"Microsoft.XMLHTTP\");\
                };\
                xmlhttp.onreadystatechange = function () {\
                    if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {\
                      document.getElementById('message_text').innerHTML=xmlhttp.responseText;\
                    };\
                };\
                xmlhttp.open(\"GET\", \"removeitem?item=\"+str, true);\
                xmlhttp.send();\
        }\
        function removeItem(i) {\
            var index=Number(i);\
            if(index){\
                var list = document.getElementById('todo_list');\
                var listItems = list.getElementsByTagName('li');\
                if (list.hasChildNodes()&&listItems.length>=i) {\
                    postRemoveItem(list.childNodes[index-1].innerHTML);\
                    list.removeChild(list.childNodes[index-1]);\
                }\
            }\
        }\
        function clickRemoveButton(){\
            removeItem(Number(remove_number.value));\
        }\
        function QueryDateAll(){\
                var xmlhttp;\
                if (window.XMLHttpRequest) {\
                    xmlhttp = new XMLHttpRequest();\
                } else {\
                    xmlhttp = new ActiveXObject(\"Microsoft.XMLHTTP\");\
                }\
                xmlhttp.onreadystatechange = function () {\
                    if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {\
                      var array=xmlhttp.responseText.split(\" \");\
                      var len=array.length;\
                      document.getElementById('todo_list').innerHTML=\"\";\
                      if(len==0)return;\
                      for(i=0;i<len-1;i++){\
                      addItem(array[i]);}\
                    };\
                };\
                xmlhttp.open(\"GET\", \"queryall\", true);\
                xmlhttp.send();\
        }\
    </script>\
</html>";
*/
#include "LittleFS.h"
bool InitLittleFS();
String ReadPage(const char* path);
String ReadHomePage();
String ReadQueryPage();
String ReadRemovePage();
String ReadNotFoundPage();