import express from 'express';
import cors from 'cors';
import bodyParser from 'body-parser';
import dotenv from 'dotenv';
import { initializeDatabase } from './config/initDb.js';
import billRoutes from './routes/billRoutes.js';
import dashboardRoutes from './routes/dashboardRoutes.js';
import deviceRoutes from './routes/deviceRoutes.js';

dotenv.config();

const app = express();
const PORT = process.env.PORT || 5000;

app.use(cors({
  origin: '*',
  credentials: true,
  methods: ['GET', 'POST', 'PUT', 'DELETE', 'OPTIONS'],
  allowedHeaders: ['Content-Type', 'Authorization']
}));
app.use(bodyParser.json({ limit: '10mb' }));
app.use(bodyParser.urlencoded({ limit: '10mb', extended: true }));

app.use((req, res, next) => {
  console.log(`📡 ${req.method} ${req.path} from ${req.ip}`);
  next();
});

const startServer = async () => {
  try {
    await initializeDatabase();
    console.log('✓ Database initialized');
  } catch (error) {
    console.error('Failed to initialize database:', error);
    process.exit(1);
  }
};

app.use('/api/bills', billRoutes);
app.use('/api/dashboard', dashboardRoutes);
app.use('/api/device', deviceRoutes);

app.get('/api/health', (req, res) => {
  res.json({
    status: 'ok',
    timestamp: new Date().toISOString(),
    service: 'Smart Billing System API'
  });
});

app.use((err, req, res, next) => {
  console.error('Unhandled error:', err);
  res.status(500).json({
    error: 'Internal server error',
    message: process.env.NODE_ENV === 'development' ? err.message : undefined
  });
});

app.use((req, res) => {
  res.status(404).json({ error: 'Endpoint not found' });
});

startServer().then(() => {
  app.listen(PORT, '0.0.0.0', () => {
    console.log(`\n╔════════════════════════════════════════╗`);
    console.log(`║  Smart Billing System Server           ║`);
    console.log(`║  Running on http://0.0.0.0:${PORT}           ║`);
    console.log(`║  Accessible at: http://10.224.248.61:${PORT} ║`);
    console.log(`╚════════════════════════════════════════╝\n`);
  });
}).catch(error => {
  console.error('Failed to start server:', error);
  process.exit(1);
});
