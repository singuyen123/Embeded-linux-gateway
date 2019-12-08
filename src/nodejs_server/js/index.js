//(function timeAgo(selector) {

/* var templates = {
    prefix: "hace ",
    suffix: "",
    seconds: "menos de un minuto",
    minute: "un minuto",
    minutes: "%d minutos",
    hour: "una hora",
    hours: "%d horas",
    day: "un dia",
    days: "%d dias",
    month: "un mes",
    months: "%d meses",
    year: "un año",
    years: "%d años"
};
var template = function(t, n) {
    return templates[t] && templates[t].replace(/%d/i, Math.abs(Math.round(n)));
};

var timer = function(time) {
    if (!time)
        return;
    time = time.replace(/\.\d+/, ""); // remove milliseconds
    time = time.replace(/-/, "/").replace(/-/, "/");
    time = time.replace(/T/, " ").replace(/Z/, " UTC");
    time = time.replace(/([\+\-]\d\d)\:?(\d\d)/, " $1$2"); // -04:00 -> -0400
    time = new Date(time * 1000 || time);

    var now = new Date();
    var seconds = ((now.getTime() - time) * .001) >> 0;
    var minutes = seconds / 60;
    var hours = minutes / 60;
    var days = hours / 24;
    var years = days / 365;

    return templates.prefix + (
            seconds < 45 && template('seconds', seconds) ||
            seconds < 90 && template('minute', 1) ||
            minutes < 45 && template('minutes', minutes) ||
            minutes < 90 && template('hour', 1) ||
            hours < 24 && template('hours', hours) ||
            hours < 42 && template('day', 1) ||
            days < 30 && template('days', days) ||
            days < 45 && template('month', 1) ||
            days < 365 && template('months', days / 30) ||
            years < 1.5 && template('year', 1) ||
            template('years', years)
            ) + templates.suffix;
};

var elements = document.getElementsByClassName('card_time');
for (var i in elements) {
    var $this = elements[i];
    if (typeof $this === 'object') {
        $this.innerHTML = timer($this.getAttribute('title') || $this.getAttribute('datetime'));
    }
}
// update time every minute
setTimeout(timeAgo, 60000);

})();

$(document).ready(function () {

	
$("#touchFlow").touchFlow();

});

//--- inicio termostato
var temperatura = 20;
$("#termostato").text(temperatura + "ºC");
function termostato(modifier) {
if (modifier == "+1") {
    temperatura++;
    $("#termostato").text(temperatura + "ºC");
} else if (modifier == "-1") {
    temperatura--;
    $("#termostato").text(temperatura + "ºC");
} else if (modifier == "+5") {
    temperatura = temperatura + 5;
    $("#termostato").text(temperatura + "ºC");
} else if (modifier == "-5") {
    temperatura = temperatura - 5;
    $("#termostato").text(temperatura + "ºC");
}
}
//--- fin termostato
*/
var socket = io();
socket.on('connect', () => {
    socket.emit('type', 'index');
    
})

function control_temperature(value) {
    // var test = document.getElementsByClassName("half-button color-white");
    // console.log("in test");
    // test.innerHTML = "sdt";
    var test = document.getElementById("termostato");
    if (value ==1) {
        test.innerHTML = "sdt";
        test.style = "#2C3539"
    }
    else test.innerHTML = "15º"
}