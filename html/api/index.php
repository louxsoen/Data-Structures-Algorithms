<?php 
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ERROR);

session_start();

function send_req($req)
{
    try
    {
        $req = json_encode($req);
        $sock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
        if(!$sock) return '{"res":-1,"msg":"Unable to create socket"}';
        $conn = socket_connect($sock, "127.0.0.1", 8080);
        if(!$conn) return '{"res":-1,"msg":"Unable to connect to Back-End Server"}';
        socket_write($sock, $reqs, strlen($reqs));
        $res = socket_read($sock, 1024);
    }
    catch(Exception $e)
    {
        return '{"res":-1, "msg":"' . $e->getMessage() . '"}';
    }
    finally
    {
        socket_close($sock);
    }
    if($res) return $res;
    return '{"res":-1, "msg":"Unknown Error occured"}';
}
if($_POST["type"] == "login") //login req
{
    if(!isset($_SESSION["ID"]))
    {
        $req["reqType"] = 1;
        $req["req"]["id"] = $_POST["id"];
        $req["req"]["passwd"] = hash("sha256", $_POST["pw"]);
    
        $res = json_decode(send_req($req));
    
        if($res->res == 1)
        {
            $_SESSION["ID"] = htmlspecialchars($_POST["id"]);
            echo('{"res":1,"msg":"Login Success"}');
        }
        else if($res->res == 2) echo('{"res":-1, "msg":"Account is not exist"}');
        else echo('{"res":-1, "msg":"' . $res->msg . '"}');
    }
    else echo('{"res":-1, "msg":"Already Logged in"}');
}
else if($_POST["type"] == "bookinfo")
{
    $req["reqType"] = 3;
    $req["req"]["isbn"] = $_POST["isbn"];

    $res = send_req($req);

    echo $res;
}
else if($_POST["type"] == "booklist")
{
    $req["reqType"] = 5;
    $req["req"]["query"] = $_POST["searchQuery"];
    $req["req"]["qType"] = $_POST["searchType"];
    $req["req"]["limit"] = (is_numeric($_POST["limit"]) && intval($_POST["limit"]) <= 20 && intval($_POST["limit"]) > 0 ) ? intval($_POST["limit"]) : 20 ;

    $res = send_req($req);

    echo $res;
}
else if($_POST["type"] == "rent")
{

}
else if($_POST["type"] == "return")
{

}
else
{

}
?>