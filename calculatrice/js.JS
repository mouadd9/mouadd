let nombres1 = ['nombre0', 'nombre1', 'nombre2', 'nombre3', 'nombre4',
    'nombre5', 'nombre6', 'nombre7', 'nombre8', 'nombre9'];
let operations = ['plus', 'moin', 'fois', 'division'];

let a = [];
let c = [];

let nombreactuel = '';
let nombreprecedent = '';
let operation = '';
let completé = false;

nombres1.forEach(function (nombre) {
    a.push(document.getElementById(nombre));
})
operations.forEach(function (nombre) {
    c.push(document.getElementById(nombre));
})

a.forEach(function (element) {
    element.addEventListener("mousedown",function(event){
        event.target.style.backgroundColor = 'rgba(39, 2, 65, 0.593)';
    })
    element.addEventListener("mouseup",function(event){
        event.target.style.backgroundColor = '';
    })
    element.addEventListener("click", function (event) {
        if (completé) {
            nombreactuel = '';
            completé = false;
        }
        nombreactuel += event.target.textContent;
        document.getElementById('resultat').innerHTML = nombreactuel;

    })
})

c.forEach(function (op) {
    op.addEventListener("mousedown",function(event){
        event.target.style.backgroundColor = 'rgba(39, 2, 65, 0.593)';
    })
    op.addEventListener("mouseup",function(event){
        event.target.style.backgroundColor = '';
    })
    op.addEventListener("click", function (event) {
        if (!completé) {
            nombreprecedent = nombreactuel;
            nombreactuel = '';
        }
        operation = event.target.textContent;
        completé = false;
        document.getElementById('resultat').innerHTML = operation;
    })
})

let E = document.getElementById('egale');
E.addEventListener("mousedown",function(event){
    event.target.style.backgroundColor = 'rgba(39, 2, 65, 0.593)';
})
E.addEventListener("mouseup",function(event){
    event.target.style.backgroundColor = '';
})
E.addEventListener('click', function () {
    if (nombreprecedent !== '' && nombreactuel !== '' && operation !== '') {
        let num1 = parseFloat(nombreprecedent);
        let num2 = parseFloat(nombreactuel);
        let resultat = 0;

        switch (operation) {
            case '+':
                resultat = num1 + num2;
                break;
            case '-':
                resultat = num1 - num2;
                break;
            case 'x':
                resultat = num1 * num2;
                break;
            case '/':
                resultat = num2 !== 0 ? num1 / num2 : 'Error';
                break;
            default:
                resultat = 'Operation invalid';
                break;
        }
        nombreprecedent = resultat.toString();
        nombreactuel = '';
        operation = '';
        document.getElementById('resultat').innerHTML = resultat;
        completé = true;
    }
});
let AC = document.getElementById('reset');
AC.addEventListener("mousedown",function(event){
    event.target.style.backgroundColor = 'rgba(39, 2, 65, 0.593)';
})
AC.addEventListener("mouseup",function(event){
    event.target.style.backgroundColor = '';
})
AC.addEventListener('click', function () {
    nombreactuel = '';
    nombreprecedent = '';
    operation = '';
    completé = false;
    document.getElementById('resultat').innerHTML = '0';
});


