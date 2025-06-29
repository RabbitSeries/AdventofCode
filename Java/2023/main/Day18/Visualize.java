package Day18;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.List;

import javax.imageio.ImageIO;

import Day18.LavaInterior.DigPlan;

public class Visualize {
    List<DigPlan> DigPlanList;

    public static void main(String[] args) throws IOException {
        LavaInterior Input = new LavaInterior();
        Visualize Vis = new Visualize();
        Vis.DigPlanList = Input.readFile();
        Vis.Solution1();
        Vis.Solution2();
    }

    final static int[] dx = new int[] {
            0, 1, 0, -1
    };

    final static int[] dy = new int[] {
            1, 0, -1, 0
    };

    public void Solution1() throws IOException {
        visualizDigplan(DigPlanList, "large_digplan1.jpg", 2000, 1);
    }

    public void Solution2() {
        for (var digPlan : DigPlanList) {
            digPlan.update();
        }
        visualizDigplan(DigPlanList, "large_digplan2.jpg", 8000, 8);
    }

    public void visualizDigplan(List<DigPlan> digPlans, String outputFilename,
        int maxDimension, int lineWidth) {
        if (digPlans == null || digPlans.isEmpty()) {
            throw new IllegalArgumentException("DigPlan list cannot be null or empty");
        }

        // First pass to calculate bounds
        long minX = 0, maxX = 0, minY = 0, maxY = 0;
        long currentX = 0, currentY = 0;

        for (DigPlan dp : digPlans) {
            int length = dp.second.first;
            currentX += dx[dp.first] * length;
            currentY += dy[dp.first] * length;

            minX = Math.min(minX, currentX);
            maxX = Math.max(maxX, currentX);
            minY = Math.min(minY, currentY);
            maxY = Math.max(maxY, currentY);
        }

        // Calculate the required scaling factor
        long spanX = maxX - minX;
        long spanY = maxY - minY;
        long maxSpan = Math.max(spanX, spanY);

        double scale = maxSpan > maxDimension ? (double) maxDimension / maxSpan : 1.0;

        // Create image with safe dimensions
        int width = (int) (spanX * scale) + 20;
        int height = (int) (spanY * scale) + 20;

        BufferedImage image = new BufferedImage(height, width, BufferedImage.TYPE_BYTE_BINARY);
        Graphics2D g2d = image.createGraphics();

        // White background
        g2d.setColor(Color.WHITE);
        g2d.fillRect(0, 0, height, width);

        // Draw the scaled path
        double x = -minX * scale + 10;
        double y = -minY * scale + 10;

        for (DigPlan dp : digPlans) {
            int direction = dp.first;
            int length = dp.second.first;
            g2d.setColor(Color.BLACK);
            g2d.setStroke(new BasicStroke(lineWidth));

            double newX = x, newY = y;
            double scaledLength = length * scale;
            newX = x + dx[direction] * scaledLength;
            newY = y + dy[direction] * scaledLength;

            g2d.drawLine((int) y, (int) x, (int) newY, (int) newX);
            x = newX;
            y = newY;
        }
        // Mark starting point
        // g2d.fillOval((int) (-minY * scale + 10 - maxDimension / 400), (int) (-minX * scale + 10 - maxDimension / 400), maxDimension / 200, maxDimension / 200);
        // g2d.fillOval((int) (lastY - maxDimension / 400), (int) (lastX - maxDimension / 400), maxDimension / 200, maxDimension / 200);
        // Save image
        try {
            ImageIO.write(image, "jpg", new File(outputFilename));
            System.out.println("Visualization saved to: " + outputFilename);
            System.out.printf("Original dimensions: %dx%d, Scaled to: %dx%d (scale: %.6f)%n",
                    spanX, spanY, width, height, scale);
        } catch (Exception e) {
            System.err.println("Error saving image: " + e.getMessage());
        } finally {
            g2d.dispose();
        }
    }
}
