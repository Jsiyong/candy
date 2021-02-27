(function(window, document, undefined) {
    var canvas, ctx, height, width, bounds,
        center, mouse, hover = false,
        mouseDown = false,
        props, particles,
        stats, gui;

    var Vector2 = function(x, y) {
        x = x || 0;
        y = y || 0;
        this.x = x;
        this.y = y;
        return this;
    };

    Vector2.prototype.add = function(vec) {
        this.x += vec.x;
        this.y += vec.y;
    };

    Vector2.prototype.sub = function(vec) {
        this.x -= vec.x;
        this.y -= vec.y;
    };

    Vector2.prototype.lerp = function(to, amount) {
        amount = amount || 0.05;
        this.x = (1 - amount) * this.x + amount * to.x;
        this.y = (1 - amount) * this.y + amount * to.y;
    };


    Vector2.prototype.distanceTo = function(vec) {
        return Math.sqrt(Math.pow(vec.x - this.x, 2) + Math.pow(vec.y - this.y, 2));
    };

    Vector2.prototype.angleTo = function(vec, format) {
        format = format || 'rad';
        var angle = format === 'rad' ? Math.atan2(vec.y - this.y, vec.x - this.x) :
            format === 'deg' ? Math.atan2(vec.y - this.y, vec.x - this.x) * 180 / Math.PI :
                undefined;
        return angle;
    };

    var Particle = function() {
        this.size = props.minSize,
            this.velocity = new Vector2(
                1 - Math.random() * 2,
                1 - Math.random() * 2
            );
        this.position = new Vector2(
            Math.random() * width,
            Math.random() * height
        );
        return this;
    };

    Particle.prototype.checkBoundaries = function() {
        if (this.position.x < bounds.xMin || this.position.x > bounds.xMax) this.velocity.x *= -1;
        if (this.position.y < bounds.yMin || this.position.y > bounds.yMax) this.velocity.y *= -1;
    };

    Particle.prototype.update = function() {
        this.position.add(this.velocity);
        this.checkBoundaries();
    };

    function resize() {
        canvas.height = height = window.innerHeight;
        canvas.width = width = window.innerWidth;
        center = new Vector2(width / 2, height / 2);
        bounds = {
            xMin: -props.mouseThreshold,
            yMin: -props.mouseThreshold,
            xMax: width + props.mouseThreshold,
            yMax: height + props.mouseThreshold
        };
    }

    function populate() {
        particles = [];
        for (var i = 0; i < props.numParticles; particles[i++] = new Particle());
    }

    function animateHover(p) {
        var mouseDist = p.position.distanceTo(mouse);
        if (mouseDist < props.mouseThreshold) {
            var mLineOpacity = 1 - mouseDist / props.mouseThreshold,
                mLineColor = props.mouseConnectColor.substr(0, props.mouseConnectColor.length - 2) + mLineOpacity + ')',
                size = props.maxSize * (1 - (mouseDist / props.mouseThreshold)) + props.minSize,
                angle = mouse.angleTo(p.position),
                position = new Vector2(
                    mouse.x + props.mouseThreshold * Math.cos(angle),
                    mouse.y + props.mouseThreshold * Math.sin(angle)
                );

            p.size = size;
            ctx.beginPath();
            ctx.strokeStyle = mLineColor;
            ctx.moveTo(mouse.x, mouse.y);
            ctx.lineTo(p.position.x, p.position.y);
            ctx.stroke();
            ctx.closePath();

            if (mouseDown)
                p.position.lerp(mouse);
            else
                p.position.lerp(position);
        } else {
            p.size = props.minSize;
        }
    }

    function draw() {
        ctx.fillStyle = props.backgroundColor;
        ctx.fillRect(0, 0, width, height);
        for (var i = 0; i < particles.length; i++) {
            var p1 = particles[i],
                p2 = particles[i + 1] || particles[0],
                distance = p2.position.distanceTo(p1.position);

            p1.update();
            ctx.fillStyle = props.particleColor;
            ctx.fillRect(p1.position.x - p1.size / 2, p1.position.y - p1.size / 2, p1.size, p1.size);
            if (distance < props.connectThreshold) {
                var pLineOpacity = 1 - distance / props.connectThreshold,
                    pLineColor = props.particleConnectColor.substr(0, props.particleConnectColor.length - 2) + pLineOpacity + ')';
                ctx.beginPath();
                ctx.strokeStyle = pLineColor;
                ctx.lineWidth = 1;
                ctx.moveTo(p1.position.x, p1.position.y);
                ctx.lineTo(p2.position.x, p2.position.y);
                ctx.stroke();
                ctx.closePath();
            }
            if (hover) {
                animateHover(p1);
            }
        }
        if (props.screenOverlay) {
            var grad = ctx.createRadialGradient(center.x, center.y, 0, center.x, center.y, width / 2);
            grad.addColorStop(0, 'rgba(0,0,0,0)');
            grad.addColorStop(1, 'rgba(0,0,0,0.6)');
            ctx.fillStyle = grad;
            ctx.fillRect(0, 0, width, height);
        }
    }

    function loop() {
        stats.begin();
        draw();
        window.requestAnimationFrame(loop);
        stats.end();
    }

    function setProps() {
        props = {
            backgroundColor: '#0c0c0f',
            particleColor: 'rgba(85,255,180,0.25)',
            minSize: 2,
            maxSize: 30,
            numParticles: 1000,
            connectThreshold: 150,
            mouseThreshold: 150,
            particleConnectColor: 'rgba(200,100,200,0)',
            mouseConnectColor: 'rgba(40,150,250,0)',
            screenOverlay: true
        };
    }

    function buildUI() {
        stats = new Stats();
        document.body.appendChild(stats.domElement);

        gui = new dat.GUI();

        var f1 = gui.addFolder('System'),
            f2 = gui.addFolder('Particles'),
            f3 = gui.addFolder('Interaction');

        f1.open();
        f2.open();
        f3.open();

        f1.addColor(props, 'backgroundColor');
        f1.add(props, 'screenOverlay');

        var updateCount = f2.add(props, 'numParticles', 100, 4000).step(20);

        f2.add(props, 'minSize', 0, 10).step(0.5);
        f2.add(props, 'maxSize', 0, 100);
        f2.addColor(props, 'particleColor');
        f2.add(props, 'connectThreshold', 0, 1000).step(5);
        f2.addColor(props, 'particleConnectColor');
        var updateMouseThreshold = f3.add(props, 'mouseThreshold', 0, 500).step(5);
        f3.addColor(props, 'mouseConnectColor');

        updateCount.onFinishChange(function() {
            populate();
        });

        updateMouseThreshold.onFinishChange(function() {
            populate(); //to avoid getting particles stuck outside of boundaries
        });
    }

    window.onload = function() {
        canvas = document.getElementById('canvas');
        ctx = canvas.getContext('2d');
        mouse = new Vector2();
        setProps();
        buildUI();
        resize();
        populate();
        loop();
    }

    window.onresize = function() {
        resize();
        populate();
    };

    window.onmousemove = function(e) {
        hover = true;
        mouse.x = e.clientX;
        mouse.y = e.clientY;
    };

    window.onmousedown = function() {
        mouseDown = true;
    };

    window.onmouseup = function() {
        mouseDown = false;
    };

    window.onmouseout = function() {
        hover = false;
    };

    window.requestAnimationFrame = (function() {
        return window.requestAnimationFrame ||
            window.webkitRequestAnimationFrame ||
            window.mozRequestAnimationFrame ||
            window.oRequestAnimationFrame ||
            window.msRequestAnimationFrame ||
            function(callback) {
                window.setTimeout(callback, 1000 / 60);
            };
    })();

})(this, document);