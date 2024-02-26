document.addEventListener('DOMContentLoaded', function() {
    
    var locations = document.querySelectorAll('.container4 .location');

    locations.forEach(function(location) {
        location.addEventListener('mouseenter', function() {
            
            var h3 = this.querySelector('h3');
            if (h3) {
                var city = h3.textContent; 
                h3.remove(); 
                
              
                var p = document.createElement('p');
            
                switch (city) {
                    case 'Rabat':
                        p.textContent = 'Agdal - Rabat Center Mall';
                        break;
                    case 'Casablanca':
                        p.textContent = 'Anfa';
                        break;
                    case 'Tetouan':
                        p.textContent = 'Martil - Tetouan Coast';
                        break;
                    default:
                        p.textContent = 'Location Detail';
                }
                
              
                this.appendChild(p);
            }
        });

        location.addEventListener('mouseleave', function() {
         
            var p = this.querySelector('p');
            if (p) p.remove(); 
            var h3 = document.createElement('h3');
            switch (location.getAttribute('data-city')) {
                case 'Rabat':
                    h3.textContent = 'Rabat';
                    break;
                case 'Casablanca':
                    h3.textContent = 'Casablanca';
                    break;
                case 'Tetouan':
                    h3.textContent = 'Tetouan';
                    break;
                default:
                    h3.textContent = 'City Name';
            }
            
           
            this.appendChild(h3);
        });
    });
});