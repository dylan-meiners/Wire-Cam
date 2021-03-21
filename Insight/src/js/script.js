window.$ = window.jQuery = require('jquery')

var WIDTH = 81;
var HEIGHT = 98.5;

var activePage = "";
var old_active_a = ".dashboard_a"
var old_page = ".dashboard"
var cToArduinoStatus = false
var cToServerStatus = false
var javaScriptToServerStatus = false
var leftPosition = 0;
var rightPosition = 0;
var leftDirection = "";
var rightDirection = "";
var x = 0;
var y = 0;

var socket = io.connect('http://localhost:3000')
socket.emit("message", "vj")

socket.on("message", ProcessMessage);

$(document).ready(function() {
    setInterval(update, 50)

    $(".nav a").click(function() {
        $(old_active_a).removeClass("nav_active")
        old_active_a = "." + $(this).attr("class")
        $(this).addClass("nav_active")
        if (old_active_a == ".dashboard_a") loadPage(".dashboard")
        else if (old_active_a == ".backend_a") loadPage(".backend")
        else (console.log("Page invalid"))
    })

    $(".dashboard").hide()
    $(".backend").hide()
    loadPage(".dashboard")
})

function loadPage(page) {
    $(old_page).hide()
    $(page).show()
    old_page = page
}

function setUpListeners(page) {
    if (page == "../html/dashboard.html") {}
    else if (page == "../html/sequencing.html") {}
    else {console.log("Invalid page")}
}

function ProcessMessage(msg) {
    switch (msg.slice(0, 1)) {
        case "c":
            leftPosition = parseInt(msg.slice(msg.indexOf(">") + 1, msg.indexOf("_1")))
            rightPosition = parseInt(msg.slice(msg.indexOf("_1") + 2, msg.indexOf("_2")))
            leftDirection = msg.slice(msg.indexOf("_2") + 2, msg.indexOf("_3"))
            rightDirection = msg.slice(msg.indexOf("_3") + 2, msg.indexOf("_4"))
            if (msg.slice(msg.indexOf("_4") + 2, msg.indexOf("_5")) == "connected") cToArduinoStatus = true;
            else cToArduinoStatus = false;
            x = msg.slice(msg.indexOf("_5") + 2, msg.indexOf("_6"))
            y = msg.slice(msg.indexOf("_6") + 2, msg.length)
            //rightPosition = parseInt()
            console.log(leftPosition + " | " + rightPosition + " | " + leftDirection + " | " + rightDirection);
            var childs = $(".console").children().toArray()
            if (childs.length >= 800) {

                childs[0].remove()
                childs[1].remove()
                childs[2].remove()
                childs[3].remove()
            }
            $(".console").append("<span style=\"color: blue\">" + msg.slice(1, msg.indexOf(">")) + "</span>")
            $(".console").append("<span style=\"color: green\">" + msg.slice(msg.indexOf(">"), msg.indexOf(">") + 1) + "</span>")
            $(".console").append("<span style=\"color: red\">" + msg.slice(msg.indexOf(">") + 1, msg.length) + "</span><div class=\"spacer\"/>")
            $(".console").scrollTop($(".console")[0].scrollHeight)
            break
        case "z":
            var m = msg.slice(2, msg.length)
            //connection change
            switch (msg.slice(1, 2)) {
                case "g":
                    if (m == "cs") cToServerStatus = true;
                    else if (m == "ca") cToArduinoStatus = true;
                    else if (m == "js") javaScriptToServerStatus = true;
                    break
                case "b":
                    if (m == "cs") cToServerStatus = false;
                    else if (m == "ca") cToArduinoStatus = false;
                    else if (m == "js") javaScriptToServerStatus = false;
                    break
                default:
                    console.log("Invalid g/b")
                    break
            }
            break
        default:
            break
    }
}

function update() {
    if (cToArduinoStatus) {
        if ($(".c_a_conn").hasClass("bad")) {
            $(".c_a_conn").removeClass("bad")
            $(".c_a_conn").addClass("good")
            $(".c_a_conn").text("Connected")
        }
    }
    else {
        if ($(".c_a_conn").hasClass("good")) {
            $(".c_a_conn").removeClass("good")
            $(".c_a_conn").addClass("bad")
            $(".c_a_conn").text("Disconnected")
        }
    }
    if (cToServerStatus) {
        if ($(".c_conn").hasClass("bad")) {
            $(".c_conn").removeClass("bad")
            $(".c_conn").addClass("good")
            $(".c_conn").text("Connected")
        }
    }
    else {
        if ($(".c_conn").hasClass("good")) {
            $(".c_conn").removeClass("good")
            $(".c_conn").addClass("bad")
            $(".c_conn").text("Disconnected")
        }
    }
    if (javaScriptToServerStatus) {
        if ($(".js_conn").hasClass("bad")) {
            $(".js_conn").removeClass("bad")
            $(".js_conn").addClass("good")
            $(".js_conn").text("Connected")
        }
    }
    else {
        if ($(".js_conn").hasClass("good")) {
            $(".js_conn").removeClass("good")
            $(".js_conn").addClass("bad")
            $(".js_conn").text("Disconnected")
        }
    }
    if (rightDirection == "out") $(".right_winch_image").attr("src", "../../media/red-sc.png")
    else if (rightDirection == "in") $(".right_winch_image").attr("src", "../../media/green-sc.png")
    else if (rightDirection == "idle") $(".right_winch_image").attr("src", "../../media/yellow-sc.png")
    else {
        $(".right_winch_image").attr("src", "../../media/blue-sc.png")
        //console.log("Invalid direction or no data")
    }
    if (leftDirection == "out") $(".left_winch_image").attr("src", "../../media/red-sc.png")
    else if (leftDirection == "in") $(".left_winch_image").attr("src", "../../media/green-sc.png")
    else if (leftDirection == "idle") $(".left_winch_image").attr("src", "../../media/yellow-sc.png")
    else {
        $(".left_winch_image").attr("src", "../../media/blue-sc.png")
        //console.log("Invalid direction or no data")
    }
    console.log("X: " + x + " | Y: " + y);
    $(".cplane").css("max-width", ($(".connections").offset().top - 10) * WIDTH / HEIGHT)
    $(".cplane").css("height",  $(".cplane").width() * HEIGHT / WIDTH)
    $(".gimbal-image").css("top", y / HEIGHT * $(".cplane-inner").height())
    $(".gimbal-image").css("left", x / WIDTH * $(".cplane-inner").width())
    //$(".gimbal").css({top: y, left: x})
}