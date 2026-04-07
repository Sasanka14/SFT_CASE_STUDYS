function toggleMenu() {
  const menu = document.querySelector(".menu");
  const nav = document.querySelector(".nav");
  menu.classList.toggle("active");
  nav.classList.toggle("active");
}

document.addEventListener("DOMContentLoaded", function () {
  // Initialize Materialize Carousel
  const carousels = document.querySelectorAll(".carousel");
  if (carousels.length > 0) {
    M.Carousel.init(carousels, {
      dist: -80,        
      shift: 0,
      padding: 40,
      numVisible: 5,
      indicators: false,
      noWrap: false
    });
  }

  // Contact Form Submission
  const contactForm = document.getElementById('contactForm');
  if (contactForm) {
    contactForm.addEventListener('submit', function(e) {
      e.preventDefault();
      alert('Thank you for your message! We will get back to you soon.');
      this.reset();
    });
  }

  // Models Page Filter
  const filterBtns = document.querySelectorAll('.filter-btn');
  const modelCards = document.querySelectorAll('.model-card');
  if (filterBtns.length > 0 && modelCards.length > 0) {
    filterBtns.forEach(btn => {
      btn.addEventListener('click', () => {
        filterBtns.forEach(b => b.classList.remove('active'));
        btn.classList.add('active');

        const filter = btn.dataset.filter;

        modelCards.forEach(card => {
          if (filter === 'all' || card.dataset.category === filter) {
            card.style.display = 'block';
            card.style.animation = 'fadeIn 0.5s ease forwards';
          } else {
            card.style.display = 'none';
          }
        });
      });
    });
  }

  // Gallery Page Filter
  const galleryFilterBtns = document.querySelectorAll('.gallery-filter-btn');
  const galleryItems = document.querySelectorAll('.gallery-item');
  if (galleryFilterBtns.length > 0 && galleryItems.length > 0) {
    galleryFilterBtns.forEach(btn => {
      btn.addEventListener('click', () => {
        galleryFilterBtns.forEach(b => b.classList.remove('active'));
        btn.classList.add('active');

        const filter = btn.dataset.filter;

        galleryItems.forEach(item => {
          if (filter === 'all' || item.dataset.category === filter) {
            item.style.display = 'block';
            item.style.animation = 'fadeIn 0.5s ease forwards';
          } else {
            item.style.display = 'none';
          }
        });
      });
    });
  }

  // About Page Stats Animation
  const statNumbers = document.querySelectorAll('.stat-number');
  const statsSection = document.querySelector('.about-stats');
  
  if (statNumbers.length > 0 && statsSection) {
    const animateStats = () => {
      statNumbers.forEach(stat => {
        const target = parseInt(stat.dataset.target);
        const increment = target / 100;
        let current = 0;
        
        const updateNumber = () => {
          if (current < target) {
            current += increment;
            stat.textContent = Math.ceil(current).toLocaleString();
            requestAnimationFrame(updateNumber);
          } else {
            stat.textContent = target.toLocaleString();
          }
        };
        
        updateNumber();
      });
    };

    const observer = new IntersectionObserver((entries) => {
      entries.forEach(entry => {
        if (entry.isIntersecting) {
          animateStats();
          observer.unobserve(entry.target);
        }
      });
    }, { threshold: 0.5 });

    observer.observe(statsSection);
  }

  // Model Detail Page
  const modelDetailPage = document.querySelector('.model-detail-page');
  if (modelDetailPage) {
    const models = {
      '765lt-spider': {
        name: '765LT Spider',
        category: 'SUPERCAR',
        tagline: 'Uncompromising Performance. Unrivaled Thrills.',
        topSpeed: '205',
        horsepower: '755',
        acceleration: '2.7',
        video: './assets/videos/mclaren-1.mp4',
        image: './assets/image/mclaren-1.jpg',
        description: 'The 765LT Spider represents the pinnacle of open-top supercar engineering. Born from McLaren\'s Longtail heritage, it delivers an exhilarating combination of lightweight construction, aerodynamic excellence, and breathtaking performance.'
      },
      'artura': {
        name: 'Artura',
        category: 'HYBRID SUPERCAR',
        tagline: 'The Future of High Performance.',
        topSpeed: '205',
        horsepower: '671',
        acceleration: '3.0',
        video: './assets/videos/mclaren-2.mp4',
        image: './assets/image/mclaren-2.jpg',
        description: 'The Artura is McLaren\'s all-new High-Performance Hybrid supercar, combining a twin-turbo V6 with an electric motor for instant torque and reduced emissions without compromising performance.'
      },
      '750s': {
        name: '750S',
        category: 'SUPERCAR',
        tagline: 'Raise Your Limits.',
        topSpeed: '206',
        horsepower: '740',
        acceleration: '2.8',
        video: './assets/videos/mclaren-3.mp4',
        image: './assets/image/mclaren-3.jpeg',
        description: 'The 750S delivers the most engaging driving experience in the supercar class. With enhanced aerodynamics, reduced weight, and increased power, every drive becomes extraordinary.'
      },
      '765lt': {
        name: '765LT',
        category: 'ULTIMATE',
        tagline: 'Longtail. Legendary.',
        topSpeed: '205',
        horsepower: '755',
        acceleration: '2.7',
        video: './assets/videos/mclaren-4.mp4',
        image: './assets/image/mclaren-4.jpeg',
        description: 'The 765LT continues the illustrious Longtail lineage with extreme aerodynamic enhancements, significant weight reduction, and driver-focused engineering for ultimate track performance.'
      },
      '750s-spider': {
        name: '750S Spider',
        category: 'SUPERCAR',
        tagline: 'Performance Unroofed.',
        topSpeed: '206',
        horsepower: '740',
        acceleration: '2.8',
        video: './assets/videos/mclaren-5.mp4',
        image: './assets/image/mclaren-5.jpeg',
        description: 'The 750S Spider combines the exhilarating performance of the 750S with the visceral thrill of open-top driving. A retractable hardtop transforms the experience in seconds.'
      }
    };

    const urlParams = new URLSearchParams(window.location.search);
    const modelId = urlParams.get('model') || '765lt-spider';
    const model = models[modelId] || models['765lt-spider'];

    document.title = `${model.name} | AXION Motors`;
    document.getElementById('modelName').textContent = model.name;
    document.getElementById('topSpeed').textContent = model.topSpeed;
    document.getElementById('horsepower').textContent = model.horsepower;
    document.getElementById('acceleration').textContent = model.acceleration;
    document.getElementById('heroVideo').querySelector('source').src = model.video;
    document.getElementById('heroVideo').load();
    document.getElementById('overviewImage').src = model.image;
    document.getElementById('modelDescription').textContent = model.description;
    document.getElementById('ctaModelName').textContent = model.name;
    document.querySelector('.model-category').textContent = model.category;
    document.querySelector('.model-tagline').textContent = model.tagline;
  }

  // Lightbox keyboard controls
  document.addEventListener('keydown', (e) => {
    const lightbox = document.getElementById('lightbox');
    if (lightbox && lightbox.classList.contains('active')) {
      if (e.key === 'Escape') closeLightbox();
      if (e.key === 'ArrowLeft') changeLightboxImage(-1);
      if (e.key === 'ArrowRight') changeLightboxImage(1);
    }
  });
});

// Video Controls for Homepage
function changeVideo(name){
  const bgVideoList = document.querySelectorAll('.bg-video');
  const trailers = document.querySelectorAll('.trailer');
  const models = document.querySelectorAll('.model');

  bgVideoList.forEach(video => {
    video.classList.remove('active');
    if(video.classList.contains(name)){
      video.classList.add('active');
    }
  });

  trailers.forEach(video => {
    video.classList.remove('active');
    if(video.classList.contains(name)){
      video.classList.add('active');
    }
  });

  models.forEach(model => {
    model.classList.remove('active');
    if(model.classList.contains(name)){
      model.classList.add('active');
    }
  });
}

function togglePlay() {
  const play = document.querySelector('.play');
  const pause = document.querySelector('.pause');
  play.classList.toggle('active');
  pause.classList.toggle('active');
}

function pauseVideo() {
  const bgVideoList = document.querySelectorAll('.bg-video');
  bgVideoList.forEach(video => {
    video.pause();
  });
  togglePlay();
}

function playVideo() {
  const bgVideoList = document.querySelectorAll('.bg-video');
  bgVideoList.forEach(video => {
    video.play();
  });
  togglePlay();
}

// Lightbox Functions for Gallery
let currentImageIndex = 0;
let galleryImages = [];

function openLightbox(button) {
  const item = button.closest('.gallery-item');
  const img = item.querySelector('img');
  
  galleryImages = Array.from(document.querySelectorAll('.gallery-item:not(.video-item) img'));
  currentImageIndex = galleryImages.indexOf(img);
  
  document.getElementById('lightbox-img').src = img.src;
  document.getElementById('lightbox').classList.add('active');
  document.body.style.overflow = 'hidden';
}

function closeLightbox() {
  document.getElementById('lightbox').classList.remove('active');
  document.body.style.overflow = 'auto';
}

function changeLightboxImage(direction) {
  currentImageIndex += direction;
  if (currentImageIndex < 0) currentImageIndex = galleryImages.length - 1;
  if (currentImageIndex >= galleryImages.length) currentImageIndex = 0;
  
  document.getElementById('lightbox-img').src = galleryImages[currentImageIndex].src;
}

// Play Gallery Video
function playGalleryVideo(button) {
  const item = button.closest('.gallery-item');
  const video = item.querySelector('video');
  
  if (video.paused) {
    video.play();
    button.innerHTML = '<i class="bi bi-pause-fill"></i>';
  } else {
    video.pause();
    button.innerHTML = '<i class="bi bi-play-fill"></i>';
  }
}
