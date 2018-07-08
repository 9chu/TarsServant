#!/bin/bash

echo "] Starting upload"

SERVER_ID=`curl -s ${1}/pages/server/api/server_list?tree_node_id=1${2}.5${3} | grep -oP '(?<=\"id\":)([0-9]+)'`

if [ -z "$SERVER_ID" ]; then
    (>&2 echo "Server ${2}.${3} not found")
    exit 1
fi

echo "] ${2}.${3} SERVER_ID=${SERVER_ID}"
echo "] Uploading patch ${3}.tgz"

if [ ! -f "${3}.tgz" ]; then
    (>&2 echo "Patch file not found")
    exit 1
fi

UPLOAD_DATE=`date "+%Y%m%d_%H%M%S"`
PATCH_ID=`curl -s -X POST -F "file=@${3}.tgz" ${1}/pages/server/api/upload_patch_package?application=${2}\&module_name=${3}\&comment=AutoUpload_${UPLOAD_DATE} | grep -oP '(?<="id":)([0-9]+)'`

if [ -z "$PATCH_ID" ]; then
    (>&2 echo "Upload patch ${2}.${3} failed")
    exit 1
fi

echo "] Deploying ${2}.${3} PATCH_ID=${PATCH_ID}"

POST_CONTENT="{\"serial\":true,\"items\":[{\"server_id\":${SERVER_ID},\"command\":\"patch_tars\",\"parameters\":{\"patch_id\":${PATCH_ID},\"update_text\":\"AutoUpload_${UPLOAD_DATE}\",\"bak_flag\":false}}]}"
RESULT=`curl -s -X POST -H "Accept: application/json" -H "Content-type: application/json" -d "${POST_CONTENT}" ${1}/pages/server/api/add_task`
RET_CODE=`echo "${RESULT}" | grep -oP '(?<="ret_code": )([0-9]+)'`
TASK_NO=`echo "${RESULT}" | grep -oP '(?<="task_no":")([0-9a-zA-Z]+)(?=","serial")'`

echo "] RET_CODE=${RET_CODE} TASK_NO=${TASK_NO}"
echo "] Done"
